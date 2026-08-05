## Messaging layer API — send operation.
import results, chronos

import logos_delivery/api/types
import logos_delivery/api/events/messaging_client_events
import logos_delivery/messaging/messaging_client
import logos_delivery/waku/waku
import logos_delivery/messaging/delivery_service/send_service
import logos_delivery/messaging/delivery_service/send_service/delivery_task

proc send*(
    self: MessagingClient, envelope: MessageEnvelope
): Future[Result[RequestId, string]] {.async.} =
  ## High-level messaging API send. Builds a `DeliveryTask` and hands it to
  ## the send service, which owns the send subscription lease. Returns the
  ## id the caller can correlate with `MessageSentEvent` / `MessageErrorEvent`.
  ?self.checkApiAvailability()

  let requestId = RequestId.new(self.waku.rng)

  let deliveryTask = DeliveryTask.new(requestId, envelope, self.waku.brokerCtx).valueOr:
    return err("MessagingClient.send: Failed to create delivery task: " & error)

  asyncSpawn self.sendService.send(deliveryTask)

  return ok(requestId)
