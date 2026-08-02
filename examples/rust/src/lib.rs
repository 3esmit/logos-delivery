use std::ffi::CString;
use std::os::raw::{c_char, c_int, c_void};
use std::slice;
use std::sync::{Arc, Condvar, Mutex};
use std::time::{Duration, Instant};

pub type FFICallback = unsafe extern "C" fn(c_int, *const c_char, usize, *const c_void);

const RET_OK: c_int = 0;
const RET_ERR: c_int = 1;
const RET_STALE_WARN: c_int = 3;

#[derive(Debug, PartialEq, Eq)]
enum RequestReply {
    Ok(Vec<u8>),
    Error { status: c_int, payload: Vec<u8> },
}

impl RequestReply {
    fn into_result(self) -> Result<Vec<u8>, String> {
        match self {
            Self::Ok(payload) => Ok(payload),
            Self::Error { status, payload } => Err(format!(
                "request callback returned {status}: {}",
                String::from_utf8_lossy(&payload)
            )),
        }
    }
}

struct RequestState {
    terminal_reply: Mutex<Option<RequestReply>>,
    terminal_callback: Condvar,
    retained_c_strings: Mutex<Vec<CString>>,
}

impl RequestState {
    fn new() -> Self {
        Self {
            terminal_reply: Mutex::new(None),
            terminal_callback: Condvar::new(),
            retained_c_strings: Mutex::new(Vec::new()),
        }
    }

    fn complete(&self, reply: RequestReply) {
        let mut terminal_reply = self
            .terminal_reply
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        if terminal_reply.is_none() {
            *terminal_reply = Some(reply);
            self.terminal_callback.notify_all();
        }
    }

    fn wait_for_terminal(&self, timeout: Duration) -> Option<RequestReply> {
        let deadline = Instant::now().checked_add(timeout)?;
        let mut terminal_reply = self
            .terminal_reply
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());

        loop {
            if terminal_reply.is_some() {
                return terminal_reply.take();
            }

            let remaining = deadline.checked_duration_since(Instant::now())?;
            let (new_reply, wait_result) = self
                .terminal_callback
                .wait_timeout(terminal_reply, remaining)
                .unwrap_or_else(|poisoned| poisoned.into_inner());
            terminal_reply = new_reply;

            if wait_result.timed_out() && terminal_reply.is_none() {
                return None;
            }
        }
    }
}

/// Owns a request callback until its one terminal response arrives.
///
/// The FFI receives one `Arc` reference through `user_data`. It is released
/// only by `request_callback` after `RET_OK` or `RET_ERR`; therefore a timeout
/// may return to Rust without invalidating a callback the FFI still owns.
pub struct RequestWaiter {
    state: Arc<RequestState>,
    user_data: *const c_void,
}

impl Default for RequestWaiter {
    fn default() -> Self {
        Self::new()
    }
}

impl RequestWaiter {
    pub fn new() -> Self {
        let state = Arc::new(RequestState::new());
        let user_data = Arc::into_raw(Arc::clone(&state)).cast::<c_void>();
        Self { state, user_data }
    }

    pub fn callback(&self) -> FFICallback {
        request_callback
    }

    pub fn user_data(&self) -> *const c_void {
        self.user_data
    }

    /// Keeps an input string alive until the FFI reaches its terminal callback.
    ///
    /// This is needed when the native call queues a pointer for asynchronous
    /// work. The string is retained even if `wait` times out, because the FFI
    /// still owns the callback state at that point.
    pub fn retain_c_string(&self, value: CString) -> *const c_char {
        let mut retained_c_strings = self
            .state
            .retained_c_strings
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        retained_c_strings.push(value);
        match retained_c_strings.last() {
            Some(value) => value.as_ptr(),
            None => std::ptr::null(),
        }
    }

    pub fn wait(self, timeout: Duration) -> Result<Vec<u8>, String> {
        self.state
            .wait_for_terminal(timeout)
            .ok_or_else(|| {
                "timed out waiting for the request callback; its FFI-owned state remains valid until a terminal callback arrives"
                    .to_owned()
            })?
            .into_result()
    }
}

fn decode_cbor_reply(payload: &[u8]) -> Option<&[u8]> {
    let initial = *payload.first()?;
    if !matches!(initial >> 5, 2 | 3) {
        return None;
    }

    let (header_len, decoded_len) = match initial & 0x1f {
        additional @ 0..=23 => (1, usize::from(additional)),
        24 => (2, usize::from(*payload.get(1)?)),
        25 => (
            3,
            usize::from(u16::from_be_bytes(payload.get(1..3)?.try_into().ok()?)),
        ),
        26 => (
            5,
            usize::try_from(u32::from_be_bytes(payload.get(1..5)?.try_into().ok()?)).ok()?,
        ),
        27 => (
            9,
            usize::try_from(u64::from_be_bytes(payload.get(1..9)?.try_into().ok()?)).ok()?,
        ),
        _ => return None,
    };

    if decoded_len != payload.len().checked_sub(header_len)? {
        return None;
    }

    Some(&payload[header_len..])
}

unsafe fn copy_callback_payload(
    buffer: *const c_char,
    buffer_len: usize,
) -> Result<Vec<u8>, Vec<u8>> {
    if buffer_len == 0 {
        return Ok(Vec::new());
    }
    if buffer.is_null() {
        return Err(b"missing callback payload".to_vec());
    }

    Ok(unsafe { slice::from_raw_parts(buffer.cast::<u8>(), buffer_len) }.to_vec())
}

fn terminal_reply(return_val: c_int, payload: Vec<u8>) -> Option<RequestReply> {
    match return_val {
        RET_STALE_WARN => None,
        RET_OK if payload.is_empty() => Some(RequestReply::Ok(payload)),
        RET_OK => decode_cbor_reply(&payload)
            .map(|reply| RequestReply::Ok(reply.to_vec()))
            .or_else(|| {
                Some(RequestReply::Error {
                    status: RET_ERR,
                    payload: b"invalid CBOR request reply".to_vec(),
                })
            }),
        RET_ERR => Some(RequestReply::Error {
            status: return_val,
            payload,
        }),
        status => Some(RequestReply::Error { status, payload }),
    }
}

/// # Safety
///
/// `data` must be the `user_data` pointer from exactly one `RequestWaiter`.
/// The FFI callback contract permits one terminal `RET_OK` or `RET_ERR` after
/// any number of `RET_STALE_WARN` progress callbacks. Event-listener payloads
/// are raw JSON and must not use this request-only callback.
pub unsafe extern "C" fn request_callback(
    return_val: c_int,
    buffer: *const c_char,
    buffer_len: usize,
    data: *const c_void,
) {
    if return_val == RET_STALE_WARN || data.is_null() {
        return;
    }

    let state = unsafe { Arc::from_raw(data.cast::<RequestState>()) };
    let reply = match unsafe { copy_callback_payload(buffer, buffer_len) } {
        Ok(payload) => terminal_reply(return_val, payload),
        Err(payload) => Some(RequestReply::Error {
            status: RET_ERR,
            payload,
        }),
    };

    if let Some(reply) = reply {
        state.complete(reply);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::thread;

    #[test]
    fn decodes_definite_cbor_text_and_bytes() {
        assert_eq!(
            decode_cbor_reply(&[0x63, b'f', b'o', b'o']),
            Some(&b"foo"[..])
        );
        assert_eq!(
            decode_cbor_reply(&[0x43, 0xff, 0x00, 0x01]),
            Some(&[0xff, 0x00, 0x01][..])
        );
    }

    #[test]
    fn rejects_non_string_and_malformed_cbor_replies() {
        assert_eq!(decode_cbor_reply(&[0x01]), None);
        assert_eq!(decode_cbor_reply(&[0x63, b'f', b'o']), None);
        assert_eq!(decode_cbor_reply(&[0x7f, 0x61, b'f', 0xff]), None);
    }

    #[test]
    fn stale_warning_does_not_consume_the_ffi_owned_state() {
        let waiter = RequestWaiter::new();
        let user_data = waiter.user_data();

        unsafe {
            request_callback(
                RET_STALE_WARN,
                b"still waiting".as_ptr().cast(),
                b"still waiting".len(),
                user_data,
            );
            request_callback(RET_OK, [0x62, b'o', b'k'].as_ptr().cast(), 3, user_data);
        }

        assert_eq!(
            waiter.wait(Duration::ZERO),
            Ok(b"ok".to_vec()),
            "the terminal callback must still own and complete the request"
        );
    }

    #[test]
    fn callback_state_survives_waiter_drop_until_its_terminal_callback() {
        let waiter = RequestWaiter::new();
        let state = Arc::clone(&waiter.state);
        let user_data = waiter.user_data();
        drop(waiter);

        unsafe {
            request_callback(RET_OK, [0x62, b'o', b'k'].as_ptr().cast(), 3, user_data);
        }

        assert_eq!(
            state.wait_for_terminal(Duration::ZERO),
            Some(RequestReply::Ok(b"ok".to_vec()))
        );
    }

    #[test]
    fn timeout_keeps_the_ffi_owned_state_valid_for_a_late_terminal_callback() {
        let waiter = RequestWaiter::new();
        let state = Arc::clone(&waiter.state);
        let user_data = waiter.user_data();

        assert!(matches!(
            waiter.wait(Duration::ZERO),
            Err(error) if error.contains("timed out waiting for the request callback")
        ));

        unsafe {
            request_callback(RET_OK, [0x62, b'o', b'k'].as_ptr().cast(), 3, user_data);
        }

        assert_eq!(
            state.wait_for_terminal(Duration::ZERO),
            Some(RequestReply::Ok(b"ok".to_vec()))
        );
    }

    #[test]
    fn retained_input_survives_waiter_drop_until_the_terminal_callback(
    ) -> Result<(), std::ffi::NulError> {
        let waiter = RequestWaiter::new();
        let state = Arc::clone(&waiter.state);
        let input = CString::new("asynchronous input")?;
        let input_ptr = waiter.retain_c_string(input);
        let user_data = waiter.user_data();
        drop(waiter);

        assert_eq!(
            unsafe { std::ffi::CStr::from_ptr(input_ptr) }.to_bytes(),
            b"asynchronous input"
        );

        unsafe {
            request_callback(RET_OK, [0x62, b'o', b'k'].as_ptr().cast(), 3, user_data);
        }

        assert_eq!(
            state.wait_for_terminal(Duration::ZERO),
            Some(RequestReply::Ok(b"ok".to_vec()))
        );
        Ok(())
    }

    #[test]
    fn background_callback_wakes_the_waiting_request() -> std::thread::Result<()> {
        let waiter = RequestWaiter::new();
        let user_data = waiter.user_data() as usize;
        let callback_thread = thread::spawn(move || unsafe {
            request_callback(
                RET_OK,
                [0x62, b'o', b'k'].as_ptr().cast(),
                3,
                user_data as *const c_void,
            );
        });

        assert_eq!(waiter.wait(Duration::from_secs(1)), Ok(b"ok".to_vec()));
        callback_thread.join()
    }

    #[test]
    fn errors_and_event_payloads_remain_raw() {
        let error = br#"{"error":"request failed"}"#;
        assert_eq!(
            terminal_reply(RET_ERR, error.to_vec()),
            Some(RequestReply::Error {
                status: RET_ERR,
                payload: error.to_vec(),
            })
        );

        let event = br#"{"event":"message_received"}"#;
        assert_eq!(
            unsafe { copy_callback_payload(event.as_ptr().cast(), event.len()) },
            Ok(event.to_vec()),
            "event listeners copy raw JSON and do not call terminal_reply"
        );
    }
}
