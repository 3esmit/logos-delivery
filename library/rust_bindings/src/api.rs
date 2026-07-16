use std::os::raw::{c_char, c_int, c_void};
use std::slice;
use std::time::Duration;
use serde::de::DeserializeOwned;
use serde::Serialize;
use super::ffi;
use super::types::*;

fn encode_cbor<T: Serialize>(value: &T) -> Result<Vec<u8>, String> {
    let mut buf = Vec::new();
    ciborium::ser::into_writer(value, &mut buf).map_err(|e| e.to_string())?;
    Ok(buf)
}

fn decode_cbor<T: DeserializeOwned>(bytes: &[u8]) -> Result<T, String> {
    ciborium::de::from_reader(bytes).map_err(|e| e.to_string())
}

type FFIResult = Result<Vec<u8>, String>;
type FFISender = flume::Sender<FFIResult>;

// Reconstruct the (ret, msg, len) tuple delivered by the C callback
// into a Result<Vec<u8>, String>: payload on success, UTF-8 message on error.
// `from_utf8_lossy` accepts non-UTF-8 error bytes by inserting U+FFFD; the
// alternative would be to dispatch a separate Err for invalid UTF-8, but the
// codegen contract is that Nim handlers emit `string` error payloads, so
// invalid UTF-8 here would be a Nim-side bug.
unsafe fn ffi_payload(ret: c_int, msg: *const c_char, len: usize) -> FFIResult {
    let bytes = if msg.is_null() || len == 0 {
        Vec::new()
    } else {
        slice::from_raw_parts(msg as *const u8, len).to_vec()
    };
    if ret == NIMFFI_RET_OK { Ok(bytes) }
    else        { Err(String::from_utf8_lossy(&bytes).into_owned()) }
}

// nim-ffi result-callback status codes (mirror ffi/ffi_types.nim).
const NIMFFI_RET_OK: c_int = 0;
const NIMFFI_RET_MISSING_CALLBACK: c_int = 2;
const NIMFFI_RET_STALE_WARN: c_int = 3;

unsafe extern "C" fn on_result(
    ret: c_int,
    msg: *const c_char,
    len: usize,
    user_data: *mut c_void,
) {
    // NIMFFI_RET_STALE_WARN (3) is a non-terminal progress ping: the request
    // is still running. This wrapper only delivers the final result, so ignore
    // it WITHOUT reclaiming the box — a terminal callback still owns the Sender.
    if ret == NIMFFI_RET_STALE_WARN { return; }

    // Take ownership of the boxed Sender — dropping it at end of scope
    // releases the only outstanding handle.
    let tx = Box::from_raw(user_data as *mut FFISender);

    // `tx.send` returns Err only if the awaiting future was dropped (and with it
    // the Receiver): e.g. tokio::time::timeout elapsed, a tokio::select! branch
    // lost the race, or the future was dropped before being awaited. This cannot
    // happen with the current rust_client demo but may occur in arbitrary
    // downstream consumers, so we discard the Err safely.
    // Given that this is invoked from a Nim thread, we can't propagate the error by panicking or
    // returning a Result. Furthermore, an API dev may intentionally set a timeout in the await,
    // in which case is also fine to discard the send error in this case because the API user will
    // handle the timeout expiry in their own code.
    // The important part is to ensure that the callback doesn't panic or block indefinitely if the
    // receiver is gone.
    let _ = tx.send(ffi_payload(ret, msg, len));
}

fn ffi_call_sync<F>(timeout: Duration, f: F) -> FFIResult
where
    F: FnOnce(ffi::FFICallback, *mut c_void) -> c_int,
{
    let (tx, rx) = flume::bounded::<FFIResult>(1);
    let raw = Box::into_raw(Box::new(tx)) as *mut c_void;
    let ret = f(on_result, raw);
    if ret == NIMFFI_RET_MISSING_CALLBACK {
        // Callback will never fire; reclaim the box to avoid a leak.
        drop(unsafe { Box::from_raw(raw as *mut FFISender) });
        return Err("RET_MISSING_CALLBACK (internal error)".into());
    }
    match rx.recv_timeout(timeout) {
        Ok(payload) => payload,
        Err(flume::RecvTimeoutError::Timeout) =>
            Err(format!("timed out after {:?}", timeout)),
        Err(flume::RecvTimeoutError::Disconnected) =>
            Err("callback channel disconnected before delivery".into()),
    }
}

async fn ffi_call_async<F>(timeout: Duration, f: F) -> FFIResult
where
    F: FnOnce(ffi::FFICallback, *mut c_void) -> c_int,
{
    let (tx, rx) = flume::bounded::<FFIResult>(1);
    let raw = Box::into_raw(Box::new(tx)) as *mut c_void;
    let ret = f(on_result, raw);
    if ret == NIMFFI_RET_MISSING_CALLBACK {
        drop(unsafe { Box::from_raw(raw as *mut FFISender) });
        return Err("RET_MISSING_CALLBACK (internal error)".into());
    }
    match tokio::time::timeout(timeout, rx.recv_async()).await {
        Ok(Ok(payload)) => payload,
        Ok(Err(_)) => Err("callback channel disconnected before delivery".into()),
        Err(_) => Err(format!("timed out after {:?}", timeout)),
    }
}

struct OnMessageSentHandler {
    f: Box<dyn Fn(&MessageSentPayload) + Send + Sync>,
}

unsafe extern "C" fn on_message_sent_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnMessageSentHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: MessageSentPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnMessageErrorHandler {
    f: Box<dyn Fn(&MessageErrorPayload) + Send + Sync>,
}

unsafe extern "C" fn on_message_error_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnMessageErrorHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: MessageErrorPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnMessagePropagatedHandler {
    f: Box<dyn Fn(&MessagePropagatedPayload) + Send + Sync>,
}

unsafe extern "C" fn on_message_propagated_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnMessagePropagatedHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: MessagePropagatedPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnMessageReceivedHandler {
    f: Box<dyn Fn(&MessageReceivedPayload) + Send + Sync>,
}

unsafe extern "C" fn on_message_received_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnMessageReceivedHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: MessageReceivedPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnConnectionStatusChangeHandler {
    f: Box<dyn Fn(&ConnectionStatusChangePayload) + Send + Sync>,
}

unsafe extern "C" fn on_connection_status_change_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnConnectionStatusChangeHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: ConnectionStatusChangePayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnTopicHealthChangeHandler {
    f: Box<dyn Fn(&TopicHealthChangePayload) + Send + Sync>,
}

unsafe extern "C" fn on_topic_health_change_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnTopicHealthChangeHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: TopicHealthChangePayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnConnectionChangeHandler {
    f: Box<dyn Fn(&ConnectionChangePayload) + Send + Sync>,
}

unsafe extern "C" fn on_connection_change_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnConnectionChangeHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: ConnectionChangePayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnChannelMessageReceivedHandler {
    f: Box<dyn Fn(&ChannelMessageReceivedPayload) + Send + Sync>,
}

unsafe extern "C" fn on_channel_message_received_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnChannelMessageReceivedHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: ChannelMessageReceivedPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnChannelMessageSentHandler {
    f: Box<dyn Fn(&ChannelMessageSentPayload) + Send + Sync>,
}

unsafe extern "C" fn on_channel_message_sent_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnChannelMessageSentHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: ChannelMessageSentPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnChannelMessageErrorHandler {
    f: Box<dyn Fn(&ChannelMessageErrorPayload) + Send + Sync>,
}

unsafe extern "C" fn on_channel_message_error_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnChannelMessageErrorHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: ChannelMessageErrorPayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

struct OnReceivedMessageHandler {
    f: Box<dyn Fn(&ReceivedMessagePayload) + Send + Sync>,
}

unsafe extern "C" fn on_received_message_trampoline(
    ret: c_int, msg: *const c_char, len: usize, ud: *mut c_void,
) {
    if ud.is_null() || ret != 0 || msg.is_null() || len == 0 {
        return;
    }
    let h = &*(ud as *const OnReceivedMessageHandler);
    let bytes = slice::from_raw_parts(msg as *const u8, len);
    #[derive(serde::Deserialize)]
    struct Envelope { payload: ReceivedMessagePayload }
    if let Ok(env) = ciborium::de::from_reader::<Envelope, _>(bytes) {
        (h.f)(&env.payload);
    }
}

#[derive(Debug, Clone, Copy)]
pub struct ListenerHandle { pub id: u64 }

/// High-level context for `LogosDelivery`.
pub struct LogosDeliveryCtx {
    ptr: *mut c_void,
    timeout: Duration,
    listeners: std::sync::Mutex<std::collections::HashMap<u64, Box<dyn std::any::Any + Send>>>,
}

// SAFETY: The `ptr` field points to an FFIContext owned by the Nim runtime.
// Every call through the generated FFI proc goes through
// `sendRequestToFFIThread` on the Nim side, which only enqueues the request
// onto a mutex-guarded MPSC queue (sound from any number of threads) and
// wakes the single FFI thread that dispatches every handler. The context is
// thus never mutated non-atomically from the caller's thread. The Nim-side
// reentrancy guard (`onFFIThread` threadvar) prevents handlers from
// re-entering the dispatcher. These invariants make it sound to mark the
// wrapper as Send + Sync.
unsafe impl Send for LogosDeliveryCtx {}
unsafe impl Sync for LogosDeliveryCtx {}

impl Drop for LogosDeliveryCtx {
    fn drop(&mut self) {
        if !self.ptr.is_null() {
            unsafe { ffi::logosdelivery_destroy(self.ptr); }
            self.ptr = std::ptr::null_mut();
        }
    }
}

impl LogosDeliveryCtx {
    pub fn create(config_json: String, timeout: Duration) -> Result<Self, String> {
        let req = LogosdeliveryCreateNodeCtorReq { config_json };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(timeout, |cb, ud| unsafe {
            let _ = ffi::logosdelivery_create_node(req_bytes.as_ptr(), req_bytes.len(), cb, ud);
            0
        })?;
        let addr_str: String = decode_cbor(&raw_bytes)?;
        let addr: usize = addr_str.parse().map_err(|e: std::num::ParseIntError| e.to_string())?;
        Ok(Self { ptr: addr as *mut c_void, timeout, listeners: std::sync::Mutex::new(std::collections::HashMap::new()) })
    }

    pub async fn new_async(config_json: String, timeout: Duration) -> Result<Self, String> {
        let req = LogosdeliveryCreateNodeCtorReq { config_json };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_async(timeout, move |cb, ud| unsafe {
            let _ = ffi::logosdelivery_create_node(req_bytes.as_ptr(), req_bytes.len(), cb, ud);
            0
        }).await?;
        let addr_str: String = decode_cbor(&raw_bytes)?;
        let addr: usize = addr_str.parse().map_err(|e: std::num::ParseIntError| e.to_string())?;
        Ok(Self { ptr: addr as *mut c_void, timeout, listeners: std::sync::Mutex::new(std::collections::HashMap::new()) })
    }

    fn add_listener_inner(
        &self,
        event_name: *const c_char,
        callback: ffi::FFICallback,
        raw: *mut c_void,
        owned: Box<dyn std::any::Any + Send>,
    ) -> ListenerHandle {
        let id = unsafe {
            ffi::logosdelivery_add_event_listener(self.ptr, event_name, callback, raw)
        };
        if id != 0 {
            self.listeners.lock().unwrap().insert(id, owned);
        }
        ListenerHandle { id }
    }

    /// Register a typed listener for `onMessageSent`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_message_sent_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&MessageSentPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnMessageSentHandler> = Box::new(OnMessageSentHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnMessageSentHandler as *mut c_void;
        self.add_listener_inner(b"onMessageSent\0".as_ptr() as *const c_char, on_message_sent_trampoline, raw, owned)
    }

    /// Register a typed listener for `onMessageError`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_message_error_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&MessageErrorPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnMessageErrorHandler> = Box::new(OnMessageErrorHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnMessageErrorHandler as *mut c_void;
        self.add_listener_inner(b"onMessageError\0".as_ptr() as *const c_char, on_message_error_trampoline, raw, owned)
    }

    /// Register a typed listener for `onMessagePropagated`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_message_propagated_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&MessagePropagatedPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnMessagePropagatedHandler> = Box::new(OnMessagePropagatedHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnMessagePropagatedHandler as *mut c_void;
        self.add_listener_inner(b"onMessagePropagated\0".as_ptr() as *const c_char, on_message_propagated_trampoline, raw, owned)
    }

    /// Register a typed listener for `onMessageReceived`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_message_received_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&MessageReceivedPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnMessageReceivedHandler> = Box::new(OnMessageReceivedHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnMessageReceivedHandler as *mut c_void;
        self.add_listener_inner(b"onMessageReceived\0".as_ptr() as *const c_char, on_message_received_trampoline, raw, owned)
    }

    /// Register a typed listener for `onConnectionStatusChange`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_connection_status_change_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&ConnectionStatusChangePayload) + Send + Sync + 'static,
    {
        let owned: Box<OnConnectionStatusChangeHandler> = Box::new(OnConnectionStatusChangeHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnConnectionStatusChangeHandler as *mut c_void;
        self.add_listener_inner(b"onConnectionStatusChange\0".as_ptr() as *const c_char, on_connection_status_change_trampoline, raw, owned)
    }

    /// Register a typed listener for `onTopicHealthChange`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_topic_health_change_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&TopicHealthChangePayload) + Send + Sync + 'static,
    {
        let owned: Box<OnTopicHealthChangeHandler> = Box::new(OnTopicHealthChangeHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnTopicHealthChangeHandler as *mut c_void;
        self.add_listener_inner(b"onTopicHealthChange\0".as_ptr() as *const c_char, on_topic_health_change_trampoline, raw, owned)
    }

    /// Register a typed listener for `onConnectionChange`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_connection_change_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&ConnectionChangePayload) + Send + Sync + 'static,
    {
        let owned: Box<OnConnectionChangeHandler> = Box::new(OnConnectionChangeHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnConnectionChangeHandler as *mut c_void;
        self.add_listener_inner(b"onConnectionChange\0".as_ptr() as *const c_char, on_connection_change_trampoline, raw, owned)
    }

    /// Register a typed listener for `onChannelMessageReceived`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_channel_message_received_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&ChannelMessageReceivedPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnChannelMessageReceivedHandler> = Box::new(OnChannelMessageReceivedHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnChannelMessageReceivedHandler as *mut c_void;
        self.add_listener_inner(b"onChannelMessageReceived\0".as_ptr() as *const c_char, on_channel_message_received_trampoline, raw, owned)
    }

    /// Register a typed listener for `onChannelMessageSent`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_channel_message_sent_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&ChannelMessageSentPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnChannelMessageSentHandler> = Box::new(OnChannelMessageSentHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnChannelMessageSentHandler as *mut c_void;
        self.add_listener_inner(b"onChannelMessageSent\0".as_ptr() as *const c_char, on_channel_message_sent_trampoline, raw, owned)
    }

    /// Register a typed listener for `onChannelMessageError`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_channel_message_error_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&ChannelMessageErrorPayload) + Send + Sync + 'static,
    {
        let owned: Box<OnChannelMessageErrorHandler> = Box::new(OnChannelMessageErrorHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnChannelMessageErrorHandler as *mut c_void;
        self.add_listener_inner(b"onChannelMessageError\0".as_ptr() as *const c_char, on_channel_message_error_trampoline, raw, owned)
    }

    /// Register a typed listener for `onReceivedMessage`. The returned handle can be
    /// passed to `remove_event_listener` to unregister.
    pub fn add_on_received_message_listener<F>(&self, handler: F) -> ListenerHandle
    where F: Fn(&ReceivedMessagePayload) + Send + Sync + 'static,
    {
        let owned: Box<OnReceivedMessageHandler> = Box::new(OnReceivedMessageHandler { f: Box::new(handler) });
        let raw = &*owned as *const OnReceivedMessageHandler as *mut c_void;
        self.add_listener_inner(b"onReceivedMessage\0".as_ptr() as *const c_char, on_received_message_trampoline, raw, owned)
    }

    /// Remove a previously-registered listener by handle. Returns true
    /// if the listener existed and was removed; false otherwise.
    pub fn remove_event_listener(&self, handle: ListenerHandle) -> bool {
        if handle.id == 0 { return false; }
        let rc = unsafe {
            ffi::logosdelivery_remove_event_listener(self.ptr, handle.id)
        };
        self.listeners.lock().unwrap().remove(&handle.id);
        rc == 0
    }

    pub fn start_node(&self) -> Result<String, String> {
        let req = LogosdeliveryStartNodeReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_start_node(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn start_node_async(&self) -> Result<String, String> {
        let req = LogosdeliveryStartNodeReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_start_node(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn stop_node(&self) -> Result<String, String> {
        let req = LogosdeliveryStopNodeReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_stop_node(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn stop_node_async(&self) -> Result<String, String> {
        let req = LogosdeliveryStopNodeReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_stop_node(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn subscribe(&self, content_topic_str: String) -> Result<String, String> {
        let req = LogosdeliverySubscribeReq { content_topic_str };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_subscribe(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn subscribe_async(&self, content_topic_str: String) -> Result<String, String> {
        let req = LogosdeliverySubscribeReq { content_topic_str };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_subscribe(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn unsubscribe(&self, content_topic_str: String) -> Result<String, String> {
        let req = LogosdeliveryUnsubscribeReq { content_topic_str };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_unsubscribe(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn unsubscribe_async(&self, content_topic_str: String) -> Result<String, String> {
        let req = LogosdeliveryUnsubscribeReq { content_topic_str };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_unsubscribe(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn send(&self, req: SendRequest) -> Result<String, String> {
        let req = LogosdeliverySendReq { req };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_send(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn send_async(&self, req: SendRequest) -> Result<String, String> {
        let req = LogosdeliverySendReq { req };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_send(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn get_available_node_info_ids(&self) -> Result<String, String> {
        let req = LogosdeliveryGetAvailableNodeInfoIdsReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_get_available_node_info_ids(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn get_available_node_info_ids_async(&self) -> Result<String, String> {
        let req = LogosdeliveryGetAvailableNodeInfoIdsReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_get_available_node_info_ids(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn get_node_info(&self, node_info_id: String) -> Result<String, String> {
        let req = LogosdeliveryGetNodeInfoReq { node_info_id };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_get_node_info(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn get_node_info_async(&self, node_info_id: String) -> Result<String, String> {
        let req = LogosdeliveryGetNodeInfoReq { node_info_id };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_get_node_info(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn get_available_configs(&self) -> Result<String, String> {
        let req = LogosdeliveryGetAvailableConfigsReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_get_available_configs(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn get_available_configs_async(&self) -> Result<String, String> {
        let req = LogosdeliveryGetAvailableConfigsReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_get_available_configs(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_peerids_from_peerstore(&self) -> Result<String, String> {
        let req = WakuGetPeeridsFromPeerstoreReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_peerids_from_peerstore(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_peerids_from_peerstore_async(&self) -> Result<String, String> {
        let req = WakuGetPeeridsFromPeerstoreReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_peerids_from_peerstore(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_connect(&self, peer_multi_addr: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuConnectReq { peer_multi_addr, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_connect(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_connect_async(&self, peer_multi_addr: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuConnectReq { peer_multi_addr, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_connect(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_disconnect_peer_by_id(&self, peer_id: String) -> Result<String, String> {
        let req = WakuDisconnectPeerByIdReq { peer_id };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_disconnect_peer_by_id(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_disconnect_peer_by_id_async(&self, peer_id: String) -> Result<String, String> {
        let req = WakuDisconnectPeerByIdReq { peer_id };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_disconnect_peer_by_id(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_disconnect_all_peers(&self) -> Result<String, String> {
        let req = WakuDisconnectAllPeersReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_disconnect_all_peers(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_disconnect_all_peers_async(&self) -> Result<String, String> {
        let req = WakuDisconnectAllPeersReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_disconnect_all_peers(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_dial_peer(&self, peer_multi_addr: String, protocol: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuDialPeerReq { peer_multi_addr, protocol, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_dial_peer(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_dial_peer_async(&self, peer_multi_addr: String, protocol: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuDialPeerReq { peer_multi_addr, protocol, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_dial_peer(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_dial_peer_by_id(&self, peer_id: String, protocol: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuDialPeerByIdReq { peer_id, protocol, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_dial_peer_by_id(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_dial_peer_by_id_async(&self, peer_id: String, protocol: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuDialPeerByIdReq { peer_id, protocol, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_dial_peer_by_id(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_connected_peers_info(&self) -> Result<String, String> {
        let req = WakuGetConnectedPeersInfoReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_connected_peers_info(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_connected_peers_info_async(&self) -> Result<String, String> {
        let req = WakuGetConnectedPeersInfoReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_connected_peers_info(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_connected_peers(&self) -> Result<String, String> {
        let req = WakuGetConnectedPeersReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_connected_peers(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_connected_peers_async(&self) -> Result<String, String> {
        let req = WakuGetConnectedPeersReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_connected_peers(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_peerids_by_protocol(&self, protocol: String) -> Result<String, String> {
        let req = WakuGetPeeridsByProtocolReq { protocol };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_peerids_by_protocol(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_peerids_by_protocol_async(&self, protocol: String) -> Result<String, String> {
        let req = WakuGetPeeridsByProtocolReq { protocol };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_peerids_by_protocol(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_discv5_update_bootnodes(&self, bootnodes: String) -> Result<String, String> {
        let req = WakuDiscv5UpdateBootnodesReq { bootnodes };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_discv5_update_bootnodes(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_discv5_update_bootnodes_async(&self, bootnodes: String) -> Result<String, String> {
        let req = WakuDiscv5UpdateBootnodesReq { bootnodes };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_discv5_update_bootnodes(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_dns_discovery(&self, enr_tree_url: String, name_dns_server: String, timeout_ms: i32) -> Result<String, String> {
        let req = WakuDnsDiscoveryReq { enr_tree_url, name_dns_server, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_dns_discovery(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_dns_discovery_async(&self, enr_tree_url: String, name_dns_server: String, timeout_ms: i32) -> Result<String, String> {
        let req = WakuDnsDiscoveryReq { enr_tree_url, name_dns_server, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_dns_discovery(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_start_discv5(&self) -> Result<String, String> {
        let req = WakuStartDiscv5Req {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_start_discv5(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_start_discv5_async(&self) -> Result<String, String> {
        let req = WakuStartDiscv5Req {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_start_discv5(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_stop_discv5(&self) -> Result<String, String> {
        let req = WakuStopDiscv5Req {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_stop_discv5(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_stop_discv5_async(&self) -> Result<String, String> {
        let req = WakuStopDiscv5Req {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_stop_discv5(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_peer_exchange_request(&self, num_peers: u64) -> Result<String, String> {
        let req = WakuPeerExchangeRequestReq { num_peers };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_peer_exchange_request(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_peer_exchange_request_async(&self, num_peers: u64) -> Result<String, String> {
        let req = WakuPeerExchangeRequestReq { num_peers };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_peer_exchange_request(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_version(&self) -> Result<String, String> {
        let req = WakuVersionReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_version(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_version_async(&self) -> Result<String, String> {
        let req = WakuVersionReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_version(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_listen_addresses(&self) -> Result<String, String> {
        let req = WakuListenAddressesReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_listen_addresses(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_listen_addresses_async(&self) -> Result<String, String> {
        let req = WakuListenAddressesReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_listen_addresses(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_my_enr(&self) -> Result<String, String> {
        let req = WakuGetMyEnrReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_my_enr(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_my_enr_async(&self) -> Result<String, String> {
        let req = WakuGetMyEnrReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_my_enr(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_my_peerid(&self) -> Result<String, String> {
        let req = WakuGetMyPeeridReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_my_peerid(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_my_peerid_async(&self) -> Result<String, String> {
        let req = WakuGetMyPeeridReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_my_peerid(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_get_metrics(&self) -> Result<String, String> {
        let req = WakuGetMetricsReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_get_metrics(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_get_metrics_async(&self) -> Result<String, String> {
        let req = WakuGetMetricsReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_get_metrics(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_is_online(&self) -> Result<String, String> {
        let req = WakuIsOnlineReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_is_online(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_is_online_async(&self) -> Result<String, String> {
        let req = WakuIsOnlineReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_is_online(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_ping_peer(&self, peer_addr: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuPingPeerReq { peer_addr, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_ping_peer(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_ping_peer_async(&self, peer_addr: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuPingPeerReq { peer_addr, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_ping_peer(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_get_peers_in_mesh(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetPeersInMeshReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_get_peers_in_mesh(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_get_peers_in_mesh_async(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetPeersInMeshReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_get_peers_in_mesh(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_get_num_peers_in_mesh(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetNumPeersInMeshReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_get_num_peers_in_mesh(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_get_num_peers_in_mesh_async(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetNumPeersInMeshReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_get_num_peers_in_mesh(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_get_connected_peers(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetConnectedPeersReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_get_connected_peers(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_get_connected_peers_async(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetConnectedPeersReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_get_connected_peers(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_get_num_connected_peers(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetNumConnectedPeersReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_get_num_connected_peers(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_get_num_connected_peers_async(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayGetNumConnectedPeersReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_get_num_connected_peers(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_add_protected_shard(&self, cluster_id: i32, shard_id: i32, public_key: String) -> Result<String, String> {
        let req = WakuRelayAddProtectedShardReq { cluster_id, shard_id, public_key };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_add_protected_shard(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_add_protected_shard_async(&self, cluster_id: i32, shard_id: i32, public_key: String) -> Result<String, String> {
        let req = WakuRelayAddProtectedShardReq { cluster_id, shard_id, public_key };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_add_protected_shard(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_subscribe(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelaySubscribeReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_subscribe(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_subscribe_async(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelaySubscribeReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_subscribe(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_unsubscribe(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayUnsubscribeReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_unsubscribe(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_unsubscribe_async(&self, pub_sub_topic: String) -> Result<String, String> {
        let req = WakuRelayUnsubscribeReq { pub_sub_topic };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_unsubscribe(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_relay_publish(&self, pub_sub_topic: String, json_waku_message: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuRelayPublishReq { pub_sub_topic, json_waku_message, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_relay_publish(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_relay_publish_async(&self, pub_sub_topic: String, json_waku_message: String, timeout_ms: u32) -> Result<String, String> {
        let req = WakuRelayPublishReq { pub_sub_topic, json_waku_message, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_relay_publish(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_default_pubsub_topic(&self) -> Result<String, String> {
        let req = WakuDefaultPubsubTopicReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_default_pubsub_topic(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_default_pubsub_topic_async(&self) -> Result<String, String> {
        let req = WakuDefaultPubsubTopicReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_default_pubsub_topic(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_content_topic(&self, app_name: String, app_version: u32, content_topic_name: String, encoding: String) -> Result<String, String> {
        let req = WakuContentTopicReq { app_name, app_version, content_topic_name, encoding };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_content_topic(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_content_topic_async(&self, app_name: String, app_version: u32, content_topic_name: String, encoding: String) -> Result<String, String> {
        let req = WakuContentTopicReq { app_name, app_version, content_topic_name, encoding };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_content_topic(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_pubsub_topic(&self, topic_name: String) -> Result<String, String> {
        let req = WakuPubsubTopicReq { topic_name };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_pubsub_topic(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_pubsub_topic_async(&self, topic_name: String) -> Result<String, String> {
        let req = WakuPubsubTopicReq { topic_name };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_pubsub_topic(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_store_query(&self, json_query: String, peer_addr: String, timeout_ms: i32) -> Result<String, String> {
        let req = WakuStoreQueryReq { json_query, peer_addr, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_store_query(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_store_query_async(&self, json_query: String, peer_addr: String, timeout_ms: i32) -> Result<String, String> {
        let req = WakuStoreQueryReq { json_query, peer_addr, timeout_ms };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_store_query(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_lightpush_publish(&self, pub_sub_topic: String, json_waku_message: String) -> Result<String, String> {
        let req = WakuLightpushPublishReq { pub_sub_topic, json_waku_message };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_lightpush_publish(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_lightpush_publish_async(&self, pub_sub_topic: String, json_waku_message: String) -> Result<String, String> {
        let req = WakuLightpushPublishReq { pub_sub_topic, json_waku_message };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_lightpush_publish(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_filter_subscribe(&self, pub_sub_topic: String, content_topics: String) -> Result<String, String> {
        let req = WakuFilterSubscribeReq { pub_sub_topic, content_topics };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_filter_subscribe(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_filter_subscribe_async(&self, pub_sub_topic: String, content_topics: String) -> Result<String, String> {
        let req = WakuFilterSubscribeReq { pub_sub_topic, content_topics };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_filter_subscribe(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_filter_unsubscribe(&self, pub_sub_topic: String, content_topics: String) -> Result<String, String> {
        let req = WakuFilterUnsubscribeReq { pub_sub_topic, content_topics };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_filter_unsubscribe(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_filter_unsubscribe_async(&self, pub_sub_topic: String, content_topics: String) -> Result<String, String> {
        let req = WakuFilterUnsubscribeReq { pub_sub_topic, content_topics };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_filter_unsubscribe(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn waku_filter_unsubscribe_all(&self) -> Result<String, String> {
        let req = WakuFilterUnsubscribeAllReq {};
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::waku_filter_unsubscribe_all(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn waku_filter_unsubscribe_all_async(&self) -> Result<String, String> {
        let req = WakuFilterUnsubscribeAllReq {};
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::waku_filter_unsubscribe_all(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn channel_create(&self, channel_id_str: String, content_topic_str: String, sender_id_str: String, encryption_str: String) -> Result<String, String> {
        let req = LogosdeliveryChannelCreateReq { channel_id_str, content_topic_str, sender_id_str, encryption_str };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_channel_create(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn channel_create_async(&self, channel_id_str: String, content_topic_str: String, sender_id_str: String, encryption_str: String) -> Result<String, String> {
        let req = LogosdeliveryChannelCreateReq { channel_id_str, content_topic_str, sender_id_str, encryption_str };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_channel_create(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn channel_exists(&self, channel_id_str: String) -> Result<bool, String> {
        let req = LogosdeliveryChannelExistsReq { channel_id_str };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_channel_exists(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<bool>(&raw_bytes)
    }

    pub async fn channel_exists_async(&self, channel_id_str: String) -> Result<bool, String> {
        let req = LogosdeliveryChannelExistsReq { channel_id_str };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_channel_exists(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<bool>(&raw_bytes)
    }

    pub fn channel_send(&self, channel_id_str: String, req: ChannelSendRequest) -> Result<String, String> {
        let req = LogosdeliveryChannelSendReq { channel_id_str, req };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_channel_send(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn channel_send_async(&self, channel_id_str: String, req: ChannelSendRequest) -> Result<String, String> {
        let req = LogosdeliveryChannelSendReq { channel_id_str, req };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_channel_send(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub fn channel_close(&self, channel_id_str: String) -> Result<String, String> {
        let req = LogosdeliveryChannelCloseReq { channel_id_str };
        let req_bytes = encode_cbor(&req)?;
        let raw_bytes = ffi_call_sync(self.timeout, |cb, ud| unsafe {
            ffi::logosdelivery_channel_close(self.ptr, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        })?;
        decode_cbor::<String>(&raw_bytes)
    }

    pub async fn channel_close_async(&self, channel_id_str: String) -> Result<String, String> {
        let req = LogosdeliveryChannelCloseReq { channel_id_str };
        let req_bytes = encode_cbor(&req)?;
        let ptr = self.ptr as usize;
        let raw_bytes = ffi_call_async(self.timeout, move |cb, ud| unsafe {
            ffi::logosdelivery_channel_close(ptr as *mut c_void, cb, ud, req_bytes.as_ptr(), req_bytes.len())
        }).await?;
        decode_cbor::<String>(&raw_bytes)
    }

}
