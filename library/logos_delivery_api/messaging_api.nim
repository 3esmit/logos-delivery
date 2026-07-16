import chronos, results, ffi
import
  logos_delivery/waku/common/base64,
  logos_delivery/waku/waku,
  logos_delivery/waku/waku_core/topics/content_topic,
  logos_delivery/api/types,
  ../declare_lib

# A typed CBOR request rather than a JSON blob the proc has to parse. Fields are
# unexported: genBindings copies field names verbatim, so an export marker would
# leak into the generated Rust as `pub payload*`; the proc reads them from this
# same module. `payload` is base64 because nim-ffi's Rust codegen emits seq[byte]
# as Vec<u8> without the serde_bytes annotation CBOR byte strings need, so native
# bytes do not round-trip yet -- a base64 string value does.
type SendRequest* {.ffi.} = object
  contentTopic: string
  payload: string
  ephemeral: bool

proc logosdeliverySubscribe*(
    lib: LogosDelivery, contentTopicStr: string
): Future[Result[string, string]] {.ffi.} =
  requireMessaging(lib, "Subscribe"):
    return err(errMsg)

  # ContentTopic is just a string type alias
  let contentTopic = ContentTopic(contentTopicStr)

  (await lib.messagingClient.subscribe(contentTopic)).isOkOr:
    let errMsg = $error
    return err("Subscribe failed: " & errMsg)

  return ok("")

proc logosdeliveryUnsubscribe*(
    lib: LogosDelivery, contentTopicStr: string
): Future[Result[string, string]] {.ffi.} =
  requireMessaging(lib, "Unsubscribe"):
    return err(errMsg)

  # ContentTopic is just a string type alias
  let contentTopic = ContentTopic(contentTopicStr)

  lib.messagingClient.unsubscribe(contentTopic).isOkOr:
    let errMsg = $error
    return err("Unsubscribe failed: " & errMsg)

  return ok("")

proc logosdeliverySend*(
    lib: LogosDelivery, req: SendRequest
): Future[Result[string, string]] {.ffi.} =
  requireMessaging(lib, "Send"):
    return err(errMsg)

  let payload = base64.decode(Base64String(req.payload)).valueOr:
    return err("invalid payload format: " & error)

  let envelope = MessageEnvelope.init(
    contentTopic = ContentTopic(req.contentTopic),
    payload = payload,
    ephemeral = req.ephemeral,
  )

  # Send the message via the messaging layer's own API.
  let requestId = (await lib.messagingClient.send(envelope)).valueOr:
    let errMsg = $error
    return err("Send failed: " & errMsg)

  return ok($requestId)
