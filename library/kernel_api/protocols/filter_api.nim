import std/[strutils, sequtils]
import chronicles, chronos, results, ffi
import
  logos_delivery,
  logos_delivery/waku/waku_core/message/message,
  logos_delivery/waku/waku_core/subscription/push_handler,
  logos_delivery/waku/waku_core/topics/pubsub_topic,
  logos_delivery/waku/waku_core/topics/content_topic,
  library/events/json_message_event,
  library/declare_lib

proc wakuFilterSubscribe*(
    lib: LogosDelivery, pubSubTopic: string, contentTopics: string
): Future[Result[string, string]] {.ffi.} =
  proc receivedMessageHandler(): FilterPushHandler =
    return proc(pubsubTopic: PubsubTopic, msg: WakuMessage) {.async.} =
      # This handler is `raises: [Defect]`, so the payload build has to be guarded.
      try:
        emitReceivedMessage(pubsubTopic, msg)
      except Exception, CatchableError:
        error "onReceivedMessage failed to emit event",
          error = getCurrentExceptionMsg()

  (
    await lib.waku.filterSubscribe(
      PubsubTopic(pubSubTopic),
      contentTopics.split(",").mapIt(ContentTopic(it)),
      FilterPushHandler(receivedMessageHandler()),
    )
  ).isOkOr:
    error "fail filter subscribe", error = error
    return err(error)
  return ok("")

proc wakuFilterUnsubscribe*(
    lib: LogosDelivery, pubSubTopic: string, contentTopics: string
): Future[Result[string, string]] {.ffi.} =
  (
    await lib.waku.filterUnsubscribe(
      PubsubTopic(pubSubTopic), contentTopics.split(",").mapIt(ContentTopic(it))
    )
  ).isOkOr:
    error "fail filter unsubscribe", error = error
    return err(error)
  return ok("")

proc wakuFilterUnsubscribeAll*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.filterUnsubscribeAll()).isOkOr:
    error "fail filter unsubscribe all", error = error
    return err(error)
  return ok("")
