use std::cell::OnceCell;
use std::ffi::CString;
use std::os::raw::{c_char, c_int, c_void};
use std::{slice, thread, time};

pub type FFICallBack = unsafe extern "C" fn(c_int, *const c_char, usize, *const c_void);

extern "C" {
    pub fn logosdelivery_create_node(
        config_json: *const u8,
        cb: FFICallBack,
        user_data: *const c_void,
    ) -> *mut c_void;

    pub fn waku_version(ctx: *const c_void, cb: FFICallBack, user_data: *const c_void) -> c_int;

    pub fn logosdelivery_start_node(
        ctx: *const c_void,
        cb: FFICallBack,
        user_data: *const c_void,
    ) -> c_int;

    pub fn waku_default_pubsub_topic(
        ctx: *mut c_void,
        cb: FFICallBack,
        user_data: *const c_void,
    ) -> *mut c_void;
}

const RET_OK: c_int = 0;
const RET_ERR: c_int = 1;
const RET_STALE_WARN: c_int = 3;

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

pub unsafe extern "C" fn trampoline<C>(
    return_val: c_int,
    buffer: *const c_char,
    buffer_len: usize,
    data: *const c_void,
) where
    C: FnMut(i32, &[u8]),
{
    let closure = &mut *(data as *mut C);
    if return_val == RET_STALE_WARN {
        return;
    }

    let payload = if buffer_len == 0 {
        &[]
    } else if buffer.is_null() {
        closure(RET_ERR, b"missing callback payload");
        return;
    } else {
        slice::from_raw_parts(buffer.cast::<u8>(), buffer_len)
    };

    if return_val == RET_OK && !payload.is_empty() {
        match decode_cbor_reply(payload) {
            Some(reply) => closure(return_val, reply),
            None => closure(RET_ERR, b"invalid CBOR request reply"),
        }
        return;
    }

    closure(return_val, payload);
}

pub fn get_trampoline<C>(_closure: &C) -> FFICallBack
where
    C: FnMut(i32, &[u8]),
{
    trampoline::<C>
}

fn main() {
    let config_json = "\
    { \
        \"mode\": \"Core\",\
        \"messagingOverrides\": { \
            \"listen-address\": \"127.0.0.1\",\
            \"tcp-port\": 60000, \
            \"nodekey\": \"0d714a1fada214dead6dc9c7274581ec20ff292451866e7d6d677dc818e8ccd2\", \
            \"log-level\": \"DEBUG\"
        }
    }";

    unsafe {
        // Create the waku node
        let closure = |ret: i32, data: &[u8]| {
            println!(
                "Ret {ret}. logosdelivery_create_node closure called {}",
                String::from_utf8_lossy(data)
            );
        };
        let cb = get_trampoline(&closure);
        let config_json_str = CString::new(config_json).unwrap();
        let ctx = logosdelivery_create_node(
            config_json_str.as_ptr() as *const u8,
            cb,
            &closure as *const _ as *const c_void,
        );

        // Extracting the current waku version
        let version: OnceCell<Vec<u8>> = OnceCell::new();
        let closure = |ret: i32, data: &[u8]| {
            println!(
                "version_closure. Ret: {ret}. Data: {}",
                String::from_utf8_lossy(data)
            );
            let _ = version.set(data.to_vec());
        };
        let cb = get_trampoline(&closure);
        let _ret = waku_version(
            &ctx as *const _ as *const c_void,
            cb,
            &closure as *const _ as *const c_void,
        );

        // Extracting the default pubsub topic
        let default_pubsub_topic: OnceCell<Vec<u8>> = OnceCell::new();
        let closure = |_ret: i32, data: &[u8]| {
            let _ = default_pubsub_topic.set(data.to_vec());
        };
        let cb = get_trampoline(&closure);
        let _ret = waku_default_pubsub_topic(ctx, cb, &closure as *const _ as *const c_void);

        println!(
            "Version: {}",
            String::from_utf8_lossy(version.get_or_init(|| unreachable!()))
        );
        println!(
            "Default pubsubTopic: {}",
            String::from_utf8_lossy(default_pubsub_topic.get_or_init(|| unreachable!()))
        );

        // Start the Waku node
        let closure = |ret: i32, data: &[u8]| {
            println!(
                "Ret {ret}. logosdelivery_start_node closure called {}",
                String::from_utf8_lossy(data)
            );
        };
        let cb = get_trampoline(&closure);
        let _ret = logosdelivery_start_node(ctx, cb, &closure as *const _ as *const c_void);
    }

    loop {
        thread::sleep(time::Duration::from_millis(10000));
    }
}

#[cfg(test)]
mod tests {
    use super::*;

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
    fn stale_warning_does_not_complete_the_callback() {
        let mut replies = Vec::new();
        let mut callback = |ret: i32, payload: &[u8]| replies.push((ret, payload.to_vec()));
        let callback_data = &mut callback as *mut _ as *const c_void;
        let callback_fn = get_trampoline(&callback);

        unsafe {
            callback_fn(
                RET_STALE_WARN,
                b"still waiting".as_ptr().cast(),
                b"still waiting".len(),
                callback_data,
            );
            callback_fn(RET_OK, [0x62, b'o', b'k'].as_ptr().cast(), 3, callback_data);
        }

        assert_eq!(replies, vec![(RET_OK, b"ok".to_vec())]);
    }
}
