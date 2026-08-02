import ctypes
import argparse
import sys

from waku_callbacks import CALLBACK_TYPE, TerminalRequest, callback_bytes, call_and_wait

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

def handle_event(ret, msg, length, user_data):
    print("Event received: %s" %
          callback_bytes(msg, length).decode("utf-8", errors="replace"))


def request_or_exit(operation, dispatch):
    try:
        return call_and_wait(operation, dispatch)
    except (RuntimeError, TimeoutError) as error:
        print("Error calling %s: %s" % (operation, error))
        sys.exit(1)

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

# Node creation
libwaku.logosdelivery_create_node.restype = ctypes.c_void_p
libwaku.logosdelivery_create_node.argtypes = [ctypes.c_char_p,
                             CALLBACK_TYPE,
                             ctypes.c_void_p]

create_request = TerminalRequest("logosdelivery_create_node")
ctx = libwaku.logosdelivery_create_node(
    bytes(json_config, 'utf-8'), create_request.callback, ctypes.c_void_p(0))
try:
    create_request.wait()
except (RuntimeError, TimeoutError) as error:
    print("Error creating node: %s" % error)
    sys.exit(1)
if not ctx:
    print("Error creating node: no node context returned")
    sys.exit(1)

# Retrieve the current version of the library
libwaku.waku_version.argtypes = [ctypes.c_void_p,
                                 CALLBACK_TYPE,
                                 ctypes.c_void_p]
version = request_or_exit(
    "waku_version",
    lambda callback: libwaku.waku_version(ctx, callback, ctypes.c_void_p(0)))
print("Git Version: %s" % version.decode("utf-8"))

# Retrieve the default pubsub topic
libwaku.waku_default_pubsub_topic.argtypes = [ctypes.c_void_p,
                                 CALLBACK_TYPE,
                                 ctypes.c_void_p]
default_pubsub_topic = request_or_exit(
    "waku_default_pubsub_topic",
    lambda callback: libwaku.waku_default_pubsub_topic(
        ctx, callback, ctypes.c_void_p(0))).decode("utf-8")
print("Default pubsub topic: %s" % default_pubsub_topic)

print("Bind addr: {}:{}".format(args.host, args.port))
print("Waku Relay enabled: {}".format(args.relay))

# Set the event callback
callback = CALLBACK_TYPE(handle_event)  # Keep event listener callback alive.

libwaku.logosdelivery_add_event_listener.argtypes = [ctypes.c_void_p,
                                                     ctypes.c_char_p,
                                                     CALLBACK_TYPE,
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
                               CALLBACK_TYPE,
                               ctypes.c_void_p]
request_or_exit(
    "logosdelivery_start_node",
    lambda callback: libwaku.logosdelivery_start_node(
        ctx, callback, ctypes.c_void_p(0)))

# Subscribe to the default pubsub topic
libwaku.waku_relay_subscribe.argtypes = [ctypes.c_void_p,
                                         CALLBACK_TYPE,
                                         ctypes.c_void_p,
                                         ctypes.c_char_p]
request_or_exit(
    "waku_relay_subscribe",
    lambda callback: libwaku.waku_relay_subscribe(
        ctx, callback, ctypes.c_void_p(0), default_pubsub_topic.encode('utf-8')))

libwaku.waku_connect.argtypes = [ctypes.c_void_p,
                                 CALLBACK_TYPE,
                                 ctypes.c_void_p,
                                 ctypes.c_char_p,
                                 ctypes.c_int]
if args.peer:
    request_or_exit(
        "waku_connect",
        lambda callback: libwaku.waku_connect(
            ctx, callback, ctypes.c_void_p(0), args.peer.encode('utf-8'), 10000))

# app = Flask(__name__)
# @app.route("/")
# def hello_world():
#     return "Hello, World!"

# Simply avoid the app to
a = input()
