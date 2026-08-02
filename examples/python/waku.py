import ctypes
import argparse
import sys

if sys.platform == "darwin":
    _lib_ext = "dylib"
elif sys.platform == "win32":
    _lib_ext = "dll"
else:
    _lib_ext = "so"

_lib_path = f"build/liblogosdelivery.{_lib_ext}"

libwaku = object
try:
    # This python script should be run from the root repo folder
    libwaku = ctypes.CDLL(_lib_path)
except OSError as e:
    print(f"Exception: {e}")
    print(f"""
The '{_lib_path}' library can be created with the next command from
the repo's root folder: `make liblogosdelivery`.

And it should build the library in '{_lib_path}'.

Therefore, make sure the library path env var points at the location that
contains the '{_lib_path}' library.
""")
    exit(1)

RET_OK = 0
RET_STALE_WARN = 3


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

    additional = header & 0x1f
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


def handle_event(ret, msg, length, user_data):
    print("Event received: %s" %
          callback_bytes(msg, length).decode("utf-8", errors="replace"))

def call_waku(func):
    ret = func()
    if (ret != 0):
        print("Error in %s. Error code: %d" % (locals().keys(), ret))
        exit(1)

# Parse params
parser = argparse.ArgumentParser(description='libwaku integration in Python.')
parser.add_argument('-d', '--host', dest='host', default='0.0.0.0',
                    help='Address this node will listen to. [=0.0.0.0]')
parser.add_argument('-p', '--port', dest='port', default=60000, required=True,
                    help='Port this node will listen to. [=60000]')
parser.add_argument('-k', '--key', dest='key', default="", required=True,
                    help="""P2P node private key as 64 char hex string.
e.g.: 364d111d729a6eb6d2e6113e163f017b5ef03a6f94c9b5b7bb1bb36fa5cb07a9""")
parser.add_argument('-r', '--relay', dest='relay', default="true",
                    help="Enable relay protocol: true|false [=true]")
parser.add_argument('--peer', dest='peer', default="",
                    help="Multiqualified libp2p address")

args = parser.parse_args()

# The next 'json_config' is the item passed to the 'logosdelivery_create_node'.
json_config = "{ \
                \"mode\": \"Core\", \
                \"messagingOverrides\": { \
                    \"listen-address\": \"%s\", \
                    \"tcp-port\": %d,           \
                    \"nodekey\": \"%s\",        \
                    \"log-level\": \"DEBUG\"    \
                } \
            }" % (args.host,
                  int(args.port),
                  args.key)

callback_type = ctypes.CFUNCTYPE(None, ctypes.c_int,
                                 ctypes.POINTER(ctypes.c_char),
                                 ctypes.c_size_t, ctypes.c_void_p)


def request_callback(operation, on_ok):
    def handle_response(ret, msg, length, user_data):
        if ret == RET_STALE_WARN:
            return

        raw = callback_bytes(msg, length)
        if ret != RET_OK:
            print("Error calling %s: %s" %
                  (operation, raw.decode("utf-8", errors="replace")))
            return

        try:
            on_ok(decode_cbor_reply(raw))
        except ValueError as error:
            print("Invalid CBOR reply from %s: %s" % (operation, error))

    return callback_type(handle_response)

# Node creation
libwaku.logosdelivery_create_node.restype = ctypes.c_void_p
libwaku.logosdelivery_create_node.argtypes = [ctypes.c_char_p,
                             callback_type,
                             ctypes.c_void_p]

create_callback = request_callback("logosdelivery_create_node", lambda reply: None)
ctx = libwaku.logosdelivery_create_node(bytes(json_config, 'utf-8'),
                                        create_callback, ctypes.c_void_p(0))

# Retrieve the current version of the library
libwaku.waku_version.argtypes = [ctypes.c_void_p,
                                 callback_type,
                                 ctypes.c_void_p]
version_callback = request_callback(
    "waku_version",
    lambda reply: print("Git Version: %s" % reply.decode("utf-8")))
libwaku.waku_version(ctx, version_callback, ctypes.c_void_p(0))

# Retrieve the default pubsub topic
default_pubsub_topic = ""


def set_default_pubsub_topic(reply):
    global default_pubsub_topic
    default_pubsub_topic = reply.decode("utf-8")
    print("Default pubsub topic: %s" % default_pubsub_topic)


libwaku.waku_default_pubsub_topic.argtypes = [ctypes.c_void_p,
                                 callback_type,
                                 ctypes.c_void_p]
default_pubsub_topic_callback = request_callback(
    "waku_default_pubsub_topic", set_default_pubsub_topic)
libwaku.waku_default_pubsub_topic(ctx, default_pubsub_topic_callback,
                                  ctypes.c_void_p(0))

print("Bind addr: {}:{}".format(args.host, args.port))
print("Waku Relay enabled: {}".format(args.relay))

# Set the event callback
callback = callback_type(handle_event) # This line is important so that the callback is not gc'ed

libwaku.logosdelivery_add_event_listener.argtypes = [ctypes.c_void_p,
                                                     ctypes.c_char_p,
                                                     callback_type,
                                                     ctypes.c_void_p]
libwaku.logosdelivery_add_event_listener.restype = ctypes.c_uint64
for event_name in [b"onMessageSent", b"onMessageError", b"onMessagePropagated",
                   b"onMessageReceived", b"onConnectionStatusChange",
                   b"onTopicHealthChange", b"onConnectionChange", b"onReceivedMessage",
                   b"onChannelMessageReceived", b"onChannelMessageSent",
                   b"onChannelMessageError"]:
    libwaku.logosdelivery_add_event_listener(ctx, event_name, callback, ctypes.c_void_p(0))

# Start the node
libwaku.logosdelivery_start_node.argtypes = [ctypes.c_void_p,
                               callback_type,
                               ctypes.c_void_p]
start_callback = request_callback("logosdelivery_start_node", lambda reply: None)
libwaku.logosdelivery_start_node(ctx, start_callback, ctypes.c_void_p(0))

# Subscribe to the default pubsub topic
libwaku.waku_relay_subscribe.argtypes = [ctypes.c_void_p,
                                         callback_type,
                                         ctypes.c_void_p,
                                         ctypes.c_char_p]
subscribe_callback = request_callback("waku_relay_subscribe", lambda reply: None)
libwaku.waku_relay_subscribe(ctx, subscribe_callback, ctypes.c_void_p(0),
                             default_pubsub_topic.encode('utf-8'))

libwaku.waku_connect.argtypes = [ctypes.c_void_p,
                                 callback_type,
                                 ctypes.c_void_p,
                                 ctypes.c_char_p,
                                 ctypes.c_int]
connect_callback = request_callback("waku_connect", lambda reply: None)
libwaku.waku_connect(ctx, connect_callback, ctypes.c_void_p(0),
                     args.peer.encode('utf-8'), 10000)

# app = Flask(__name__)
# @app.route("/")
# def hello_world():
#     return "Hello, World!"

# Simply avoid the app to
a = input()
