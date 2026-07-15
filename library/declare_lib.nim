import ffi
import results
import logos_delivery
import
  logos_delivery/waku/common/base64,
  logos_delivery/waku/waku_core/message,
  logos_delivery/waku/waku_core/message/digest

declareLibrary("logosdelivery", LogosDelivery)

template requireMessaging*(lib: LogosDelivery, opName: string, onError: untyped) =
  ## Fails if the node has no messaging client (a kernel-only / fleet node).
  lib.ensureMessaging().isOkOr:
    let errMsg {.inject.} = opName & " failed: " & error
    onError

template requireChannels*(lib: LogosDelivery, opName: string, onError: untyped) =
  ## Fails if the node has no reliable channel manager (a kernel-only / fleet node).
  lib.ensureChannels().isOkOr:
    let errMsg {.inject.} = opName & " failed: " & error
    onError

# Outgoing event payloads. These mirror the domain event types but hold only
# wire-friendly scalars, so they serialise through the library's ABI format and
# surface as typed listeners in the generated bindings. Byte fields are
# base64-encoded strings.
#
# The fields are deliberately unexported: genBindings copies field names
# verbatim, so an export marker would leak into the generated bindings as
# `pub payload*: String`. The `emit*` procs below are the construction path.

type WakuMessagePayload* {.ffi.} = object
  payload: string
  contentTopic: string
  version: uint32
  timestamp: int64
  ephemeral: bool
  meta: string
  proof: string

type MessageSentPayload* {.ffi.} = object
  requestId: string
  messageHash: string

type MessageErrorPayload* {.ffi.} = object
  requestId: string
  messageHash: string
  error: string

type MessagePropagatedPayload* {.ffi.} = object
  requestId: string
  messageHash: string

type MessageReceivedPayload* {.ffi.} = object
  messageHash: string
  message: WakuMessagePayload

type ConnectionStatusChangePayload* {.ffi.} = object
  connectionStatus: string

type TopicHealthChangePayload* {.ffi.} = object
  pubsubTopic: string
  topicHealth: string

type ConnectionChangePayload* {.ffi.} = object
  peerId: string
  peerEvent: string

type ChannelMessageReceivedPayload* {.ffi.} = object
  channelId: string
  senderId: string
  payload: string

type ChannelMessageSentPayload* {.ffi.} = object
  channelId: string
  requestId: string

type ChannelMessageErrorPayload* {.ffi.} = object
  channelId: string
  requestId: string
  error: string

type ReceivedMessagePayload* {.ffi.} = object
  pubsubTopic: string
  messageHash: string
  wakuMessage: WakuMessagePayload

proc onMessageSent(e: MessageSentPayload) {.ffiEvent: "onMessageSent".}
proc onMessageError(e: MessageErrorPayload) {.ffiEvent: "onMessageError".}
proc onMessagePropagated(e: MessagePropagatedPayload) {.ffiEvent: "onMessagePropagated".}
proc onMessageReceived(e: MessageReceivedPayload) {.ffiEvent: "onMessageReceived".}
proc onConnectionStatusChange(
  e: ConnectionStatusChangePayload
) {.ffiEvent: "onConnectionStatusChange".}

proc onTopicHealthChange(e: TopicHealthChangePayload) {.ffiEvent: "onTopicHealthChange".}
proc onConnectionChange(e: ConnectionChangePayload) {.ffiEvent: "onConnectionChange".}
proc onChannelMessageReceived(
  e: ChannelMessageReceivedPayload
) {.ffiEvent: "onChannelMessageReceived".}

proc onChannelMessageSent(
  e: ChannelMessageSentPayload
) {.ffiEvent: "onChannelMessageSent".}

proc onChannelMessageError(
  e: ChannelMessageErrorPayload
) {.ffiEvent: "onChannelMessageError".}

proc onReceivedMessage(e: ReceivedMessagePayload) {.ffiEvent: "onReceivedMessage".}

proc toWakuMessagePayload(msg: WakuMessage): WakuMessagePayload =
  return WakuMessagePayload(
    payload: string(base64.encode(msg.payload)),
    contentTopic: msg.contentTopic,
    version: uint32(msg.version),
    timestamp: int64(msg.timestamp),
    ephemeral: msg.ephemeral,
    meta: string(base64.encode(msg.meta)),
    proof: string(base64.encode(msg.proof)),
  )

proc emitMessageSent*(requestId, messageHash: string) =
  onMessageSent(MessageSentPayload(requestId: requestId, messageHash: messageHash))

proc emitMessageError*(requestId, messageHash, error: string) =
  onMessageError(
    MessageErrorPayload(
      requestId: requestId, messageHash: messageHash, error: error
    )
  )

proc emitMessagePropagated*(requestId, messageHash: string) =
  onMessagePropagated(
    MessagePropagatedPayload(requestId: requestId, messageHash: messageHash)
  )

proc emitMessageReceived*(messageHash: string, msg: WakuMessage) =
  onMessageReceived(
    MessageReceivedPayload(
      messageHash: messageHash, message: toWakuMessagePayload(msg)
    )
  )

proc emitConnectionStatusChange*(connectionStatus: string) =
  onConnectionStatusChange(
    ConnectionStatusChangePayload(connectionStatus: connectionStatus)
  )

proc emitTopicHealthChange*(pubsubTopic, topicHealth: string) =
  onTopicHealthChange(
    TopicHealthChangePayload(pubsubTopic: pubsubTopic, topicHealth: topicHealth)
  )

proc emitConnectionChange*(peerId, peerEvent: string) =
  onConnectionChange(ConnectionChangePayload(peerId: peerId, peerEvent: peerEvent))

proc emitChannelMessageReceived*(channelId, senderId: string, payload: seq[byte]) =
  onChannelMessageReceived(
    ChannelMessageReceivedPayload(
      channelId: channelId,
      senderId: senderId,
      payload: string(base64.encode(payload)),
    )
  )

proc emitChannelMessageSent*(channelId, requestId: string) =
  onChannelMessageSent(
    ChannelMessageSentPayload(channelId: channelId, requestId: requestId)
  )

proc emitChannelMessageError*(channelId, requestId, error: string) =
  onChannelMessageError(
    ChannelMessageErrorPayload(
      channelId: channelId, requestId: requestId, error: error
    )
  )

proc emitReceivedMessage*(pubSubTopic: string, msg: WakuMessage) =
  onReceivedMessage(
    ReceivedMessagePayload(
      pubsubTopic: pubSubTopic,
      messageHash: computeMessageHash(pubSubTopic, msg).to0xHex(),
      wakuMessage: toWakuMessagePayload(msg),
    )
  )
