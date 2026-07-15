import std/json
import chronos, chronicles, results, ffi
import libp2p/peerid # pull PeerId pretty string formatting
import logos_delivery/waku/common/base64
import
  logos_delivery,
  logos_delivery/waku/node/waku_node,
  logos_delivery/api/types,
  logos_delivery/waku/api/events/health_events,
  logos_delivery/waku/api/events/peer_events,
  logos_delivery/api/conf/logos_delivery_conf_json,
  ../declare_lib,
  ../json_event

# Add JSON serialization for RequestId
proc `%`*(id: RequestId): JsonNode =
  %($id)

proc logosdeliveryCreateNode*(
    configJson: string
): Future[Result[LogosDelivery, string]] {.ffiCtor.} =
  let conf = parseLogosDeliveryConf(configJson).valueOr:
    error "Failed to parse Logos Delivery configuration JSON",
      error = error, configJson = configJson
    return err("failed parseLogosDeliveryConf " & error)

  return await LogosDelivery.new(conf)

proc logosdeliveryStartNode*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  # setting up outgoing event listeners
  let sentListener = MessageSentEvent.listen(
    lib.waku.brokerCtx,
    proc(event: MessageSentEvent) {.async: (raises: []).} =
      emitMessageSent($event.requestId, event.messageHash),
  ).valueOr:
    chronicles.error "MessageSentEvent.listen failed", err = $error
    return err("MessageSentEvent.listen failed: " & $error)

  let errorListener = MessageErrorEvent.listen(
    lib.waku.brokerCtx,
    proc(event: MessageErrorEvent) {.async: (raises: []).} =
      emitMessageError($event.requestId, event.messageHash, event.error),
  ).valueOr:
    chronicles.error "MessageErrorEvent.listen failed", err = $error
    return err("MessageErrorEvent.listen failed: " & $error)

  let propagatedListener = MessagePropagatedEvent.listen(
    lib.waku.brokerCtx,
    proc(event: MessagePropagatedEvent) {.async: (raises: []).} =
      emitMessagePropagated($event.requestId, event.messageHash),
  ).valueOr:
    chronicles.error "MessagePropagatedEvent.listen failed", err = $error
    return err("MessagePropagatedEvent.listen failed: " & $error)

  let receivedListener = MessageReceivedEvent.listen(
    lib.waku.brokerCtx,
    proc(event: MessageReceivedEvent) {.async: (raises: []).} =
      emitMessageReceived(event.messageHash, event.message),
  ).valueOr:
    chronicles.error "MessageReceivedEvent.listen failed", err = $error
    return err("MessageReceivedEvent.listen failed: " & $error)

  let ConnectionStatusChangeListener = EventConnectionStatusChange.listen(
    lib.waku.brokerCtx,
    proc(event: EventConnectionStatusChange) {.async: (raises: []).} =
      emitConnectionStatusChange($event.connectionStatus),
  ).valueOr:
    chronicles.error "ConnectionStatusChange.listen failed", err = $error
    return err("ConnectionStatusChange.listen failed: " & $error)

  let shardTopicHealthListener = EventShardTopicHealthChange.listen(
    lib.waku.brokerCtx,
    proc(event: EventShardTopicHealthChange) {.async: (raises: []).} =
      emitTopicHealthChange($event.topic, $event.health),
  ).valueOr:
    chronicles.error "EventShardTopicHealthChange.listen failed", err = $error
    return err("EventShardTopicHealthChange.listen failed: " & $error)

  let peerEventListener = WakuPeerEvent.listen(
    lib.waku.brokerCtx,
    proc(event: WakuPeerEvent) {.async: (raises: []).} =
      emitConnectionChange($event.peerId, $event.kind),
  ).valueOr:
    chronicles.error "WakuPeerEvent.listen failed", err = $error
    return err("WakuPeerEvent.listen failed: " & $error)

  let channelReceivedListener = ChannelMessageReceivedEvent.listen(
    lib.waku.brokerCtx,
    proc(event: ChannelMessageReceivedEvent) {.async: (raises: []).} =
      emitChannelMessageReceived(
        string(event.channelId), $event.senderId, event.payload
      ),
  ).valueOr:
    chronicles.error "ChannelMessageReceivedEvent.listen failed", err = $error
    return err("ChannelMessageReceivedEvent.listen failed: " & $error)

  let channelSentListener = ChannelMessageSentEvent.listen(
    lib.waku.brokerCtx,
    proc(event: ChannelMessageSentEvent) {.async: (raises: []).} =
      emitChannelMessageSent(string(event.channelId), $event.requestId),
  ).valueOr:
    chronicles.error "ChannelMessageSentEvent.listen failed", err = $error
    return err("ChannelMessageSentEvent.listen failed: " & $error)

  let channelErrorListener = ChannelMessageErrorEvent.listen(
    lib.waku.brokerCtx,
    proc(event: ChannelMessageErrorEvent) {.async: (raises: []).} =
      emitChannelMessageError(
        string(event.channelId), $event.requestId, event.error
      ),
  ).valueOr:
    chronicles.error "ChannelMessageErrorEvent.listen failed", err = $error
    return err("ChannelMessageErrorEvent.listen failed: " & $error)

  (await lib.start()).isOkOr:
    let errMsg = $error
    chronicles.error "START_NODE failed", err = errMsg
    return err("failed to start: " & errMsg)
  return ok("")

proc logosdeliveryStopNode*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  await MessageErrorEvent.dropAllListeners(lib.waku.brokerCtx)
  await MessageSentEvent.dropAllListeners(lib.waku.brokerCtx)
  await MessagePropagatedEvent.dropAllListeners(lib.waku.brokerCtx)
  await MessageReceivedEvent.dropAllListeners(lib.waku.brokerCtx)
  await EventConnectionStatusChange.dropAllListeners(lib.waku.brokerCtx)
  await EventShardTopicHealthChange.dropAllListeners(lib.waku.brokerCtx)
  await WakuPeerEvent.dropAllListeners(lib.waku.brokerCtx)
  await ChannelMessageReceivedEvent.dropAllListeners(lib.waku.brokerCtx)
  await ChannelMessageSentEvent.dropAllListeners(lib.waku.brokerCtx)
  await ChannelMessageErrorEvent.dropAllListeners(lib.waku.brokerCtx)

  (await lib.stop()).isOkOr:
    let errMsg = $error
    chronicles.error "STOP_NODE failed", err = errMsg
    return err("failed to stop: " & errMsg)
  return ok("")

proc logosdeliveryDestroy*(lib: LogosDelivery) {.ffiDtor.} =
  discard
