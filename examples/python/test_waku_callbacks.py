import ctypes
import unittest

from waku_callbacks import (
    RET_OK,
    RET_STALE_WARN,
    TerminalRequest,
    _PENDING_REQUESTS,
    call_and_wait,
)


def invoke(request, status, payload=b""):
    buffer = ctypes.create_string_buffer(payload)
    request._handle_response(
        status,
        ctypes.cast(buffer, ctypes.POINTER(ctypes.c_char)),
        len(payload),
        None,
    )


class TerminalRequestTest(unittest.TestCase):
    def test_stale_warning_does_not_finish_request(self):
        request = TerminalRequest("version")

        invoke(request, RET_STALE_WARN, b"5000")
        self.assertFalse(request._event.is_set())

        invoke(request, RET_OK, b"\x63foo")
        self.assertEqual(request.wait(0), b"foo")

    def test_terminal_error_is_reported_after_stale_warning(self):
        request = TerminalRequest("start")

        invoke(request, RET_STALE_WARN, b"5000")
        invoke(request, 1, b"node failed")

        with self.assertRaisesRegex(RuntimeError, "node failed"):
            request.wait(0)

    def test_timeout_keeps_callback_alive_until_terminal_response(self):
        request = TerminalRequest("create")

        with self.assertRaises(TimeoutError):
            request.wait(0)
        self.assertIn(request, _PENDING_REQUESTS)

        invoke(request, RET_OK)
        self.assertEqual(request.wait(0), b"")
        self.assertNotIn(request, _PENDING_REQUESTS)

    def test_direct_dispatch_error_releases_callback_state(self):
        with self.assertRaisesRegex(RuntimeError, "rejected before callback dispatch"):
            call_and_wait("start", lambda _callback: 1, timeout_seconds=0)
        self.assertFalse(_PENDING_REQUESTS)

    def test_direct_dispatch_error_preserves_synchronous_callback_detail(self):
        def dispatch(callback):
            payload = ctypes.create_string_buffer(b"node failed")
            callback(
                1,
                ctypes.cast(payload, ctypes.POINTER(ctypes.c_char)),
                len(b"node failed"),
                None,
            )
            return 1

        with self.assertRaisesRegex(RuntimeError, "node failed"):
            call_and_wait("start", dispatch, timeout_seconds=0)
        self.assertFalse(_PENDING_REQUESTS)


if __name__ == "__main__":
    unittest.main()
