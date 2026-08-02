"""Safe terminal-callback handling for the liblogosdelivery Python example."""

import ctypes
import threading


RET_OK = 0
RET_STALE_WARN = 3

CALLBACK_TYPE = ctypes.CFUNCTYPE(
    None,
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_char),
    ctypes.c_size_t,
    ctypes.c_void_p,
)
_PENDING_REQUESTS = set()


def callback_bytes(msg, length):
    if not msg or length == 0:
        return b""
    return ctypes.string_at(msg, length)


def decode_cbor_reply(raw):
    if not raw:
        return b""

    header = raw[0]
    if header >> 5 not in (2, 3):
        raise ValueError("reply is not a CBOR text or byte string")

    additional = header & 0x1F
    if additional < 24:
        header_len = 1
        payload_len = additional
    elif 24 <= additional <= 27:
        length_len = 1 << (additional - 24)
        header_len = 1 + length_len
        if len(raw) < header_len:
            raise ValueError("truncated CBOR string header")
        payload_len = int.from_bytes(raw[1:header_len], "big")
    else:
        raise ValueError("unsupported CBOR string header")

    if len(raw) != header_len + payload_len:
        raise ValueError("CBOR string length does not match callback length")
    return raw[header_len:]


class TerminalRequest:
    """Keeps a ctypes callback alive until its one terminal response arrives."""

    def __init__(self, operation):
        self._operation = operation
        self._event = threading.Event()
        self._lock = threading.Lock()
        self._terminal = False
        self._status = None
        self._payload = b""
        self._error = None
        self.callback = CALLBACK_TYPE(self._handle_response)
        _PENDING_REQUESTS.add(self)

    def _handle_response(self, ret, msg, length, _user_data):
        if ret == RET_STALE_WARN:
            return

        with self._lock:
            if self._terminal:
                return
            self._terminal = True

            raw = callback_bytes(msg, length)
            self._status = ret
            if ret == RET_OK:
                try:
                    self._payload = decode_cbor_reply(raw)
                except ValueError as error:
                    self._error = "invalid CBOR reply: %s" % error
            else:
                self._error = raw.decode("utf-8", errors="replace")

            self._event.set()
            _PENDING_REQUESTS.discard(self)

    def wait(self, timeout_seconds=30):
        if not self._event.wait(timeout_seconds):
            # The native callback can still arrive. Keep this object and its
            # ctypes callback alive until then rather than exposing freed state.
            raise TimeoutError(
                "%s did not produce a terminal callback within %s seconds"
                % (self._operation, timeout_seconds)
            )

        if self._status != RET_OK:
            raise RuntimeError(
                "%s failed: %s" % (self._operation, self._error or self._status)
            )
        if self._error:
            raise RuntimeError("%s failed: %s" % (self._operation, self._error))
        return self._payload

    def has_terminal_callback(self):
        with self._lock:
            return self._terminal

    def abandon_before_dispatch(self):
        """Release a request only when its FFI call was never dispatched."""
        with self._lock:
            self._terminal = True
            _PENDING_REQUESTS.discard(self)


def call_and_wait(operation, dispatch, timeout_seconds=30):
    """Dispatch an asynchronous FFI request and wait for its terminal result."""
    request = TerminalRequest(operation)
    try:
        status = dispatch(request.callback)
    except BaseException:
        request.abandon_before_dispatch()
        raise
    if status not in (None, RET_OK):
        if request.has_terminal_callback():
            return request.wait(0)
        request.abandon_before_dispatch()
        raise RuntimeError(
            "%s was rejected before callback dispatch (status %s)"
            % (operation, status)
        )
    return request.wait(timeout_seconds)
