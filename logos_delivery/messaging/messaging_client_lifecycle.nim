## Messaging layer core: the `MessagingClient` type plus its construction and
## lifecycle. The public operations (subscribe / unsubscribe / send) live in
## `messaging/api.nim`.
import results, chronos
import chronicles
import
  logos_delivery/messaging/messaging_client,
  logos_delivery/messaging/api/send,
  logos_delivery/api/messaging_client_api,
  logos_delivery/waku/waku,
  logos_delivery/waku/api/subscriptions,
  logos_delivery/messaging/delivery_service/[recv_service, send_service]

# Surfaces the messaging API interface (and its Message* events) to consumers.
export messaging_client

proc start*(self: MessagingClient): Result[void, string] =
  if self.started:
    return ok()

  MessagingSend.setProvider(
    self.brokerCtx,
    proc(envelope: MessageEnvelope): Future[Result[RequestId, string]] {.async.} =
      return await self.send(envelope),
  ).isOkOr:
    return err(error)

  MessagingSubscribe.setProvider(
    self.brokerCtx,
    proc(contentTopic: ContentTopic): Result[void, string] =
      self.waku.subscribe(contentTopic),
  ).isOkOr:
    MessagingSend.clearProvider(self.brokerCtx)
    return err(error)

  MessagingUnsubscribe.setProvider(
    self.brokerCtx,
    proc(contentTopic: ContentTopic): Result[void, string] =
      self.waku.unsubscribe(contentTopic),
  ).isOkOr:
    MessagingSubscribe.clearProvider(self.brokerCtx)
    MessagingSend.clearProvider(self.brokerCtx)
    return err(error)

  self.recvService.startRecvService()
  self.sendService.startSendService()
  self.started = true
  ok()

proc stop*(self: MessagingClient) {.async.} =
  if not self.started:
    return
  MessagingSend.clearProvider(self.brokerCtx)
  MessagingSubscribe.clearProvider(self.brokerCtx)
  MessagingUnsubscribe.clearProvider(self.brokerCtx)
  await self.sendService.stopSendService()
  await self.recvService.stopRecvService()
  self.started = false
