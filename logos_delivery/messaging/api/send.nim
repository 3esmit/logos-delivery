## Messaging layer API — send operation.
import results, chronos

import logos_delivery/api/types
import logos_delivery/api/events/messaging_client_events
import logos_delivery/messaging/messaging_client
import logos_delivery/waku/waku
import logos_delivery/waku/api/subscriptions
import logos_delivery/messaging/delivery_service/send_service
import logos_delivery/messaging/delivery_service/send_service/delivery_task

proc send*(
    self: MessagingClient, envelope: MessageEnvelope
): Future[Result[RequestId, string]] {.async.} =
  ## High-level messaging API send. Acquires the content-topic lease before
  ## building a `DeliveryTask`, then hands the task to the send service.
  ## Returns the id the caller can correlate with `MessageSentEvent` /
  ## `MessageErrorEvent`.
  ?self.checkApiAvailability()

  self.waku.subscribeSend(envelope.contentTopic).isOkOr:
    return err("MessagingClient.send: Failed to auto-subscribe: " & error)

  let requestId = RequestId.new(self.waku.rng)

  let deliveryTask = DeliveryTask.new(requestId, envelope, self.waku.brokerCtx).valueOr:
    self.waku.unsubscribeSend(envelope.contentTopic).isOkOr:
      warn "MessagingClient.send: failed to release auto-subscribe lease", error = error
    return err("MessagingClient.send: Failed to create delivery task: " & error)
  deliveryTask.sendSubscriptionAcquired = true

  asyncSpawn self.sendService.send(deliveryTask)

  return ok(requestId)
