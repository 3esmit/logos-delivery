import chronos, results, ffi
import
  logos_delivery/waku/common/base64,
  logos_delivery,
  logos_delivery/channels/encryption/noop_encryption,
  logos_delivery/waku/waku_core/topics/content_topic,
  logos_delivery/api/types,
  ../declare_lib

# Typed CBOR request; `payload` is base64 for the same reason as SendRequest in
# messaging_api. Fields unexported to keep the export marker out of the bindings.
type ChannelSendRequest* {.ffi.} = object
  payload: string
  ephemeral: bool

proc installEncryption(mechanism: string): Result[void, string] =
  ## The Encrypt/Decrypt brokers dispatch to a single provider, so this installs
  ## it process-wide even though the mechanism is named per channel: channels
  ## created with different mechanisms would fight over it, last one winning.
  ## Traffic cannot flow until some provider is registered.
  case mechanism
  of "noop":
    setNoopEncryption()
    ok()
  else:
    err("unknown encryption mechanism '" & mechanism & "'; supported: noop")

proc logosdeliveryChannelCreate*(
    lib: LogosDelivery,
    channelIdStr: string,
    contentTopicStr: string,
    senderIdStr: string,
    encryptionStr: string,
): Future[Result[string, string]] {.ffi.} =
  requireChannels(lib, "ChannelCreate"):
    return err(errMsg)

  installEncryption(encryptionStr).isOkOr:
    return err("ChannelCreate failed: " & error)

  let id = lib.reliableChannelManager.createReliableChannel(
    ChannelId(channelIdStr),
    ContentTopic(contentTopicStr),
    SdsParticipantID(senderIdStr),
  ).valueOr:
    return err("ChannelCreate failed: " & $error)

  return ok(string(id))

proc logosdeliveryChannelSend*(
    lib: LogosDelivery, channelIdStr: string, req: ChannelSendRequest
): Future[Result[string, string]] {.ffi.} =
  requireChannels(lib, "ChannelSend"):
    return err(errMsg)

  let payload = base64.decode(Base64String(req.payload)).valueOr:
    return err("invalid payload format: " & error)

  let requestId = (
    await lib.reliableChannelManager.send(
      ChannelId(channelIdStr), payload, req.ephemeral
    )
  ).valueOr:
    return err("ChannelSend failed: " & $error)

  return ok($requestId)

proc logosdeliveryChannelClose*(
    lib: LogosDelivery, channelIdStr: string
): Future[Result[string, string]] {.ffi.} =
  requireChannels(lib, "ChannelClose"):
    return err(errMsg)

  (await lib.reliableChannelManager.closeChannel(ChannelId(channelIdStr))).isOkOr:
    return err("ChannelClose failed: " & $error)

  return ok("")
