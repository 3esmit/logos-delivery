import std/[strutils, json]
import chronicles, chronos, results, ffi
import
  logos_delivery,
  logos_delivery/waku/waku_core/topics/pubsub_topic,
  logos_delivery/waku/waku_core/message,
  logos_delivery/waku/waku_relay/protocol,
  library/events/json_message_event,
  library/declare_lib

proc wakuRelayGetPeersInMesh*(
    lib: LogosDelivery, pubSubTopic: string
): Future[Result[string, string]] {.ffi.} =
  let peers = (await lib.waku.relayPeersInMesh(PubsubTopic(pubSubTopic))).valueOr:
    error "LIST_MESH_PEERS failed", error = error
    return err(error)
  ## returns a comma-separated string of peerIDs
  return ok(peers.join(","))

proc wakuRelayGetNumPeersInMesh*(
    lib: LogosDelivery, pubSubTopic: string
): Future[Result[string, string]] {.ffi.} =
  let n = (await lib.waku.relayNumPeersInMesh(PubsubTopic(pubSubTopic))).valueOr:
    error "NUM_MESH_PEERS failed", error = error
    return err(error)
  return ok($n)

proc wakuRelayGetConnectedPeers*(
    lib: LogosDelivery, pubSubTopic: string
): Future[Result[string, string]] {.ffi.} =
  ## Returns the list of all connected peers to an specific pubsub topic
  let peers = (await lib.waku.relayConnectedPeers(PubsubTopic(pubSubTopic))).valueOr:
    error "LIST_CONNECTED_PEERS failed", error = error
    return err(error)
  return ok(peers.join(","))

proc wakuRelayGetNumConnectedPeers*(
    lib: LogosDelivery, pubSubTopic: string
): Future[Result[string, string]] {.ffi.} =
  let n = (await lib.waku.relayNumConnectedPeers(PubsubTopic(pubSubTopic))).valueOr:
    error "NUM_CONNECTED_PEERS failed", error = error
    return err(error)
  return ok($n)

proc wakuRelayAddProtectedShard*(
    lib: LogosDelivery, clusterId: int32, shardId: int32, publicKey: string
): Future[Result[string, string]] {.ffi.} =
  ## Protects a shard with a public key
  (
    await lib.waku.relayAddProtectedShard(
      uint16(clusterId), uint16(shardId), publicKey
    )
  ).isOkOr:
    return err(error)
  return ok("")

proc wakuRelaySubscribe*(
    lib: LogosDelivery, pubSubTopic: string
): Future[Result[string, string]] {.ffi.} =
  proc receivedMessageHandler(): WakuRelayHandler =
    return proc(pubsubTopic: PubsubTopic, msg: WakuMessage) {.async.} =
      # This handler is `raises: [Defect]`, so the payload build has to be guarded.
      try:
        emitReceivedMessage(pubsubTopic, msg)
      except Exception, CatchableError:
        error "onReceivedMessage failed to emit event",
          error = getCurrentExceptionMsg()

  (
    await lib.waku.relaySubscribe(
      PubsubTopic(pubSubTopic), WakuRelayHandler(receivedMessageHandler())
    )
  ).isOkOr:
    error "SUBSCRIBE failed", error = error
    return err(error)
  return ok("")

proc wakuRelayUnsubscribe*(
    lib: LogosDelivery, pubSubTopic: string
): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.relayUnsubscribe(PubsubTopic(pubSubTopic))).isOkOr:
    error "UNSUBSCRIBE failed", error = error
    return err(error)
  return ok("")

proc wakuRelayPublish*(
    lib: LogosDelivery, pubSubTopic: string, jsonWakuMessage: string, timeoutMs: uint32
): Future[Result[string, string]] {.ffi.} =
  var jsonMessage: JsonMessage
  try:
    let jsonContent = parseJson(jsonWakuMessage)
    jsonMessage = JsonMessage.fromJsonNode(jsonContent).valueOr:
      raise newException(JsonParsingError, $error)
  except JsonParsingError as exc:
    return err("Error parsing json message: " & exc.msg)

  let msg = json_message_event.toWakuMessage(jsonMessage).valueOr:
    return err("Problem building the WakuMessage: " & $error)

  let msgHash = (
    await lib.waku.relayPublish(PubsubTopic(pubSubTopic), msg, uint32(timeoutMs))
  ).valueOr:
    error "PUBLISH failed", error = error
    return err(error)
  return ok(msgHash)

proc wakuDefaultPubsubTopic*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  let topic = (await lib.waku.defaultPubsubTopic()).valueOr:
    return err(error)
  return ok(string(topic))

proc wakuContentTopic*(
    lib: LogosDelivery,
    appName: string,
    appVersion: uint32,
    contentTopicName: string,
    encoding: string,
): Future[Result[string, string]] {.ffi.} =
  let topic = (
    await lib.waku.buildContentTopic(
      appName, uint32(appVersion), contentTopicName, encoding
    )
  ).valueOr:
    return err(error)
  return ok(string(topic))

proc wakuPubsubTopic*(
    lib: LogosDelivery, topicName: string
): Future[Result[string, string]] {.ffi.} =
  let topic = (await lib.waku.buildPubsubTopic(topicName)).valueOr:
    return err(error)
  return ok(string(topic))
