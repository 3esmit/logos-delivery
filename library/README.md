# Logos Messaging API (LMAPI) Library

A C FFI library providing a simplified interface to Logos Messaging functionality.

## Overview

This library wraps the high-level API functions from `waku/api/api.nim` and exposes them via a C FFI interface, making them accessible from C, C++, and other languages that support C FFI.

## API Functions

### Node Lifecycle

#### `logosdelivery_create_node`
Creates a new instance of the node from the given configuration JSON.

```c
void *logosdelivery_create_node(
    const char *configJson,
    FFICallBack callback,
    void *userData
);
```

**Parameters:**
- `configJson`: JSON string containing node configuration
- `callback`: Callback function to receive the result
- `userData`: User data passed to the callback

**Returns:** Pointer to the context needed by other API functions, or NULL on error.

**Example configuration JSON:**
```json
{
  "mode": "Core",
  "preset": "logos.dev",
  "messagingOverrides": {
    "listen-address": "0.0.0.0",
    "tcp-port": 60000,
    "discv5-udp-port": 9000
  }
}
```

The configuration object has four optional top-level keys: `mode` (`"Core"` or
`"Edge"`, defaults to `"Core"`), `preset`, `messagingOverrides` (per-field node
config overrides), and `channelsOverrides` (reliable-channel overrides).
Override keys accept the config field name or its CLI switch name (e.g.
`"clusterId"` or `"cluster-id"`); unknown keys are rejected.
Use `"preset"` to select a network preset (e.g., `"twn"`, `"logos.dev"`,
`"status.prod"`) which auto-configures entry nodes, cluster ID, sharding, and
other network-specific settings.

Available presets:

| Preset | Cluster ID | RLN | Sharding | Network |
| --- | --- | --- | --- | --- |
| `twn` | 1 | on | auto (8 shards) | The Waku Network |
| `logos.dev` | 2 | off | auto (8 shards) | Logos Dev Network |
| `logos.test` | 2 | off | auto (8 shards) | Logos Test Network |
| `status.prod` | 16 | off | auto (1 shard) | Status Production Network |

#### `logosdelivery_start_node`
Starts the node.

```c
int logosdelivery_start_node(
    void *ctx,
    FFICallBack callback,
    void *userData
);
```

#### `logosdelivery_stop_node`
Stops the node while preserving restartable resources, including its REST
listener. Call `logosdelivery_start_node` to resume it on the same context.

```c
int logosdelivery_stop_node(
    void *ctx,
    FFICallBack callback,
    void *userData
);
```

#### `logosdelivery_destroy`
Destroys a node instance and frees its resources. This is final: it stops the
node, releases its HTTP listeners, and invokes the callback only after teardown
finishes. Do not reuse `ctx` after calling it.

```c
int logosdelivery_destroy(
    void *ctx,
    FFICallBack callback,
    void *userData
);
```

### Messaging

#### `logosdelivery_subscribe`
Subscribe to a content topic to receive messages.

```c
int logosdelivery_subscribe(
    void *ctx,
    FFICallBack callback,
    void *userData,
    const char *contentTopic
);
```

**Parameters:**
- `ctx`: Context pointer from `logosdelivery_create_node`
- `callback`: Callback function to receive the result
- `userData`: User data passed to the callback
- `contentTopic`: Content topic string (e.g., "/myapp/1/chat/proto")

#### `logosdelivery_unsubscribe`
Unsubscribe from a content topic.

```c
int logosdelivery_unsubscribe(
    void *ctx,
    FFICallBack callback,
    void *userData,
    const char *contentTopic
);
```

#### `logosdelivery_send`
Send a message.

```c
int logosdelivery_send(
    void *ctx,
    FFICallBack callback,
    void *userData,
    const char *messageJson
);
```

**Parameters:**
- `messageJson`: JSON string containing the message

**Example message JSON:**
```json
{
  "contentTopic": "/myapp/1/chat/proto",
  "payload": "SGVsbG8gV29ybGQ=",
  "ephemeral": false
}
```

Note: The `payload` field should be base64-encoded.

**Returns:** Request ID in the callback message that can be used to track message delivery.

### Events

Events are delivered through a per-event listener registry: register one callback
per event name you care about. A registration returns a listener id you can later
pass to remove it.

#### `logosdelivery_add_event_listener`
Registers `callback` for the named event and returns a non-zero listener id (0 on
an invalid context).

```c
uint64_t logosdelivery_add_event_listener(
    void *ctx,
    const char *eventName,
    FFICallBack callback,
    void *userData
);
```

Event names: `onMessageSent`, `onMessageError`, `onMessagePropagated`,
`onMessageReceived`, `onConnectionStatusChange`, `onTopicHealthChange`,
`onConnectionChange`, `onReceivedMessage`, `onChannelMessageReceived`,
`onChannelMessageSent`, `onChannelMessageError`.

#### `logosdelivery_remove_event_listener`
Removes a previously registered listener. Returns `0` on success, `1` if the
listener id was not found or the context is invalid.

```c
int logosdelivery_remove_event_listener(
    void *ctx,
    uint64_t listenerId
);
```

**Important:** Callbacks run on a dedicated event thread and should be fast,
non-blocking, and thread-safe.

## Building

The library follows the same build system as the main Logos Messaging project.

### Build the library

```bash
make liblogosdeliveryStatic    # Build static library
# or
make liblogosdeliveryDynamic   # Build dynamic library
```

## Return Codes

All functions that return `int` use the following return codes:

- `RET_OK` (0): Success
- `RET_ERR` (1): Error
- `RET_MISSING_CALLBACK` (2): Missing callback function
- `RET_STALE_WARN` (3): Request still in progress. This callback-only status is
  non-terminal and may be emitted repeatedly before one terminal `RET_OK` or
  `RET_ERR`; do not wake a waiter, resolve a promise, or otherwise complete the
  request for this status.

## Callback Function

All API functions use the following callback signature:

```c
typedef void (*FFICallBack)(
    int callerRet,
    const char *msg,
    size_t len,
    void *userData
);
```

**Parameters:**
- `callerRet`: Return code (RET_OK, RET_ERR, etc.)
- `msg`: Response message (may be empty for success)
- `len`: Length of the message
- `userData`: User data passed in the original call

### Request callback payloads

For request callbacks, a non-empty terminal `RET_OK` payload is a
length-delimited CBOR text string. Decode it before treating it as JSON, text,
or an identifier, and use `len` rather than C-string operations on the callback
buffer. A zero-length `RET_OK` payload is valid. `RET_ERR` payloads and
`RET_STALE_WARN` progress payloads are raw UTF-8 text.

The convenience helper in `ffi_callback.h` decodes one definite-length CBOR
text or byte string without allocating. Its result points into the callback
buffer, so copy it before the callback returns if it must outlive that call.

Event-listener callbacks are different: their `RET_OK` payloads are raw event
JSON, not CBOR request replies. See [MESSAGE_EVENTS.md](MESSAGE_EVENTS.md).

## Example Usage

```c
#include "liblogosdelivery.h"
#include "ffi_callback.h"
#include <stdio.h>

void callback(int ret, const char *msg, size_t len, void *userData) {
    if (ret == RET_STALE_WARN) {
        return; // Progress only: keep waiting for RET_OK or RET_ERR.
    }

    if (ret == RET_OK) {
        const char *payload;
        size_t payload_len;
        if (!logosdelivery_decode_cbor_reply(msg, len, &payload, &payload_len)) {
            fprintf(stderr, "Invalid CBOR request reply\n");
            return;
        }
        fputs("Success: ", stdout);
        if (payload_len != 0) {
            fwrite(payload, 1, payload_len, stdout);
        }
        fputc('\n', stdout);
    } else {
        fputs("Error: ", stdout);
        if (len != 0) {
            fwrite(msg, 1, len, stdout);
        }
        fputc('\n', stdout);
    }
}

int main() {
    const char *config = "{"
        "\"logLevel\": \"INFO\","
        "\"mode\": \"Core\","
        "\"preset\": \"logos.dev\""
        "}";

    // Create node
    void *ctx = logosdelivery_create_node(config, callback, NULL);
    if (ctx == NULL) {
        return 1;
    }

    // Start node
    logosdelivery_start_node(ctx, callback, NULL);

    // Subscribe to a topic
    logosdelivery_subscribe(ctx, callback, NULL, "/myapp/1/chat/proto");

    // Send a message
    const char *msg = "{"
        "\"contentTopic\": \"/myapp/1/chat/proto\","
        "\"payload\": \"SGVsbG8gV29ybGQ=\","
        "\"ephemeral\": false"
        "}";
    logosdelivery_send(ctx, callback, NULL, msg);

    // Clean up
    logosdelivery_stop_node(ctx, callback, NULL);
    logosdelivery_destroy(ctx, callback, NULL);

    return 0;
}
```

## Architecture

The library is structured as follows:

- `liblogosdelivery.h`: C header file with function declarations
- `liblogosdelivery.nim`: Main library entry point
- `declare_lib.nim`: Library declaration and initialization
- `lmapi/node_api.nim`: Node lifecycle API implementation
- `lmapi/messaging_api.nim`: Subscribe/send API implementation

The library uses the nim-ffi framework for FFI infrastructure, which handles:
- Thread-safe request processing
- Async operation management
- Memory management between C and Nim
- Callback marshaling

## See Also

- Main API documentation: `waku/api/api.nim`
- Original libwaku library: `library/libwaku.nim`
- nim-ffi framework: `vendor/nim-ffi/`
