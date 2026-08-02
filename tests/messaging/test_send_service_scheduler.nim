{.used.}

import std/net
import chronos, chronicles, testutils/unittests, results, stew/byteutils

import
  logos_delivery/waku/waku,
  logos_delivery/waku/waku_core,
  logos_delivery/waku/api/publish,
  logos_delivery/waku/rln/constants,
  logos_delivery/api/types,
  logos_delivery/api/events/messaging_client_events,
  logos_delivery/api/conf/messaging_conf,
  logos_delivery/waku/factory/waku_conf,
  logos_delivery/messaging/rate_limit_manager/rate_limit_manager,
  logos_delivery/messaging/delivery_service/send_service/
    [send_service, send_processor, delivery_task, relay_processor]
import ../testlib/testasync

## Scheduler-level coverage for the send service's rate-limit seam: a task is
## charged exactly once however many rounds it takes, and an over-budget task
## is parked then released when the epoch rolls. A fake processor scripts the
## delivery outcome so the loop runs without network or sleeps.

type FakeSendProcessor = ref object of BaseSendProcessor
  calls: int
  script: seq[DeliveryState]
    ## State to stamp on the task per invocation; the last entry repeats.

method process(self: FakeSendProcessor, task: DeliveryTask): Future[void] {.async.} =
  let outcome = self.script[min(self.calls, self.script.high)]
  inc self.calls
  task.state = outcome
  if outcome == DeliveryState.SuccessfullyPropagated and
      task.firstPropagatedTime.isNone():
    task.firstPropagatedTime = Opt.some(Moment.now())

proc testConf(): WakuConf =
  var conf = MessagingClientConf()
    .toWakuNodeConf(messaging_conf.LogosDeliveryMode.Core).valueOr:
      raiseAssert error
  conf.listenAddress = parseIpAddress("0.0.0.0")
  conf.tcpPort = Port(0)
  conf.discv5UdpPort = Port(0)
  conf.clusterId = Opt.some(3'u16)
  conf.numShardsInNetwork = 1
  conf.rest = false
  return conf.toWakuConf().valueOr:
    raiseAssert error

proc fixedEpochQuota(epoch: ptr uint64, userMessageLimit: uint64): QuotaProvider =
  ## Quota pinned to whatever `epoch` holds, so a test rolls the epoch by
  ## writing through the pointer.
  return proc(): Opt[EpochQuota] {.gcsafe, raises: [].} =
    return Opt.some(EpochQuota(epochIndex: epoch[], userMessageLimit: userMessageLimit))

suite "SendService - rate-limit scheduling":
  var waku {.threadvar.}: Waku

  asyncSetup:
    waku = (await Waku.new(testConf())).expect("Waku.new")

  asyncTeardown:
    ## The node is never started, so stop is best-effort cleanup.
    discard await waku.stop()

  proc buildTask(
      id, payload: string,
      timestamp: Timestamp = 1_700_000_000_000_000_000,
      proof: seq[byte] = @[],
  ): DeliveryTask =
    ## Built directly rather than via `DeliveryTask.new`, which needs a broker
    ## provider only registered once the node starts.
    let msg = WakuMessage(
      contentTopic: "/test/1/scheduler/proto",
      payload: payload.toBytes(),
      timestamp: timestamp,
      proof: proof,
    )
    let pubsubTopic = PubsubTopic("/waku/2/rs/3/0")
    return DeliveryTask(
      requestId: RequestId(id),
      pubsubTopic: pubsubTopic,
      msg: msg,
      msgHash: computeMessageHash(pubsubTopic, msg),
      state: DeliveryState.Entry,
    )

  asyncTest "a task is charged once even when delivery takes several rounds":
    ## First round fails to propagate, second succeeds. The retry must not draw a
    ## second slot: `firstAdmittedTime` guards re-admission.
    var epoch = 5'u64
    let manager = RateLimitManager
      .new(
        RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 3),
        fixedEpochQuota(addr epoch, userMessageLimit = 100),
      )
      .expect("RateLimitManager.new")
    let processor = FakeSendProcessor(
      script: @[DeliveryState.NextRoundRetry, DeliveryState.SuccessfullyPropagated]
    )
    let service =
      SendService.new(false, waku, manager, processor).expect("SendService.new")

    let task = buildTask("charge-once", "hi")
    await service.send(task)
    check:
      manager.sentInCurrentEpoch == 1'u64
      task.firstAdmittedTime.isSome()
      task.state == DeliveryState.NextRoundRetry

    await service.trySendMessages()
    check:
      manager.sentInCurrentEpoch == 1'u64 # not re-charged on retry
      processor.calls == 2
      task.state == DeliveryState.SuccessfullyPropagated

  asyncTest "an over-budget task is parked, then released when the epoch rolls":
    ## Budget of one per epoch. The second send is parked until the epoch rolls,
    ## then admitted and delivered.
    var epoch = 1'u64
    let manager = RateLimitManager
      .new(
        RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1),
        fixedEpochQuota(addr epoch, userMessageLimit = 100),
      )
      .expect("RateLimitManager.new")
    let processor = FakeSendProcessor(script: @[DeliveryState.SuccessfullyPropagated])
    let service =
      SendService.new(false, waku, manager, processor).expect("SendService.new")

    let first = buildTask("in-budget", "one")
    await service.send(first)
    check:
      first.state == DeliveryState.SuccessfullyPropagated
      manager.sentInCurrentEpoch == 1'u64

    let second = buildTask("over-budget", "two")
    await service.send(second)
    check:
      second.state == DeliveryState.NextRoundRetry # parked
      second.firstAdmittedTime.isNone() # never admitted
    let callsWhenParked = processor.calls

    # Same epoch: still over budget, so the parked task is not handed to the
    # processor.
    await service.trySendMessages()
    check:
      second.state == DeliveryState.NextRoundRetry
      second.firstAdmittedTime.isNone()
      processor.calls == callsWhenParked

    # Epoch rolls: budget refills, the parked task is admitted and delivered.
    epoch = 2'u64
    await service.trySendMessages()
    check:
      second.firstAdmittedTime.isSome()
      second.state == DeliveryState.SuccessfullyPropagated

  asyncTest "first admission after a budget park refreshes the message identity":
    var epoch = 1'u64
    let manager = RateLimitManager
      .new(
        RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1),
        fixedEpochQuota(addr epoch, userMessageLimit = 100),
      )
      .expect("RateLimitManager.new")
    let processor = FakeSendProcessor(script: @[DeliveryState.SuccessfullyPropagated])
    let service =
      SendService.new(false, waku, manager, processor).expect("SendService.new")

    await service.send(buildTask("budget-used", "one"))
    let oldTimestamp = Timestamp(1_700_000_000_000_000_000)
    let parked = buildTask("parked", "two", oldTimestamp, @[1'u8, 2'u8])
    let oldHash = parked.msgHash

    await service.send(parked)
    check:
      parked.state == DeliveryState.NextRoundRetry
      parked.firstAdmittedTime.isNone()
      parked.rateLimitParked

    epoch = 2'u64
    await service.trySendMessages()
    check:
      parked.firstAdmittedTime.isSome()
      not parked.rateLimitParked
      parked.msg.timestamp > oldTimestamp
      parked.msg.proof.len == 0
      parked.msgHash != oldHash
      parked.msgHash == computeMessageHash(parked.pubsubTopic, parked.msg)

  asyncTest "ordinary retries preserve a proof and one admission":
    var epoch = 1'u64
    let manager = RateLimitManager
      .new(
        RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1),
        fixedEpochQuota(addr epoch, userMessageLimit = 100),
      )
      .expect("RateLimitManager.new")
    let processor = FakeSendProcessor(
      script: @[DeliveryState.NextRoundRetry, DeliveryState.SuccessfullyPropagated]
    )
    let service =
      SendService.new(false, waku, manager, processor).expect("SendService.new")
    let task = buildTask("proof-reuse", "payload", proof = @[9'u8, 8'u8])
    let originalProof = task.msg.proof
    let originalHash = task.msgHash

    await service.send(task)
    await service.trySendMessages()
    check:
      manager.sentInCurrentEpoch == 1'u64
      processor.calls == 2
      task.msg.proof == originalProof
      task.msgHash == originalHash

  asyncTest "only explicit stale-proof errors reset the proof":
    let stale: ErrorStatus = (
      code: LightPushErrorCode.OUT_OF_RLN_PROOF,
      desc: Opt.some(RlnProofRefreshScheduledMsg),
    )
    let ordinary: ErrorStatus = (
      code: LightPushErrorCode.OUT_OF_RLN_PROOF,
      desc: Opt.some("proof generation failed"),
    )
    let rawRlnValidatorError: ErrorStatus =
      (code: LightPushErrorCode.INVALID_MESSAGE, desc: Opt.some(RlnValidatorErrorMsg))
    let unrelatedInvalidMessage: ErrorStatus =
      (code: LightPushErrorCode.INVALID_MESSAGE, desc: Opt.some("message is malformed"))
    check:
      stale.isStaleRlnProof()
      not ordinary.isStaleRlnProof()
      not rawRlnValidatorError.isStaleRlnProof()
      not unrelatedInvalidMessage.isStaleRlnProof()

    let task = buildTask("stale-proof", "payload", proof = @[7'u8])
    let originalHash = task.msgHash
    task.firstAdmittedTime = Opt.some(Moment.now())
    let firstAdmission = task.firstAdmittedTime
    task.retryWithFreshRlnProof()
    check:
      task.msg.proof.len == 0
      task.firstAdmittedTime == firstAdmission
      task.msgHash == originalHash

  asyncTest "a stale-proof retry reuses its quota admission":
    var epoch = 1'u64
    let manager = RateLimitManager
      .new(
        RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1),
        fixedEpochQuota(addr epoch, userMessageLimit = 100),
      )
      .expect("RateLimitManager.new")
    let processor = FakeSendProcessor(
      script: @[DeliveryState.NextRoundRetry, DeliveryState.SuccessfullyPropagated]
    )
    let service =
      SendService.new(false, waku, manager, processor).expect("SendService.new")
    let task = buildTask("stale-proof-retry", "payload", proof = @[7'u8])

    await service.send(task)
    let firstAdmission = task.firstAdmittedTime
    task.retryWithFreshRlnProof()
    await service.trySendMessages()

    check:
      firstAdmission.isSome()
      task.firstAdmittedTime == firstAdmission
      manager.sentInCurrentEpoch == 1'u64
      processor.calls == 2
      task.state == DeliveryState.SuccessfullyPropagated

  asyncTest "relay does not retry a raw RLN validator rejection":
    let rawRlnValidatorErrorPublish: PushMessageHandler = proc(
        pubsubTopic: PubsubTopic, message: WakuMessage
    ): Future[WakuLightPushResult] {.async.} =
      return lighpushErrorResult(
        LightPushErrorCode.INVALID_MESSAGE, RlnValidatorErrorMsg & ": stale merkle path"
      )
    let processor =
      RelaySendProcessor.new(false, rawRlnValidatorErrorPublish, waku, waku.brokerCtx)
    let task = buildTask("relay-stale-proof", "payload", proof = @[7'u8])
    task.firstAdmittedTime = Opt.some(Moment.now())
    let firstAdmission = task.firstAdmittedTime
    let originalProof = task.msg.proof

    await processor.process(task)

    check:
      task.state == DeliveryState.FailedToDeliver
      task.msg.proof == originalProof
      task.firstAdmittedTime == firstAdmission

  asyncTest "relay does not retry an unrelated invalid message":
    let invalidRelayPublish: PushMessageHandler = proc(
        pubsubTopic: PubsubTopic, message: WakuMessage
    ): Future[WakuLightPushResult] {.async.} =
      return
        lighpushErrorResult(LightPushErrorCode.INVALID_MESSAGE, "message is malformed")
    let processor =
      RelaySendProcessor.new(false, invalidRelayPublish, waku, waku.brokerCtx)
    let task = buildTask("relay-invalid-message", "payload", proof = @[7'u8])
    task.firstAdmittedTime = Opt.some(Moment.now())
    let originalProof = task.msg.proof
    let firstAdmission = task.firstAdmittedTime

    await processor.process(task)

    check:
      task.state == DeliveryState.FailedToDeliver
      task.msg.proof == originalProof
      task.firstAdmittedTime == firstAdmission

  asyncTest "a full rate-limit queue emits an error without caching the task":
    var epoch = 1'u64
    let manager = RateLimitManager
      .new(
        RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1),
        fixedEpochQuota(addr epoch, userMessageLimit = 100),
      )
      .expect("RateLimitManager.new")
    let processor = FakeSendProcessor(script: @[DeliveryState.SuccessfullyPropagated])
    let service = SendService
      .new(false, waku, manager, processor, maxPendingRateLimitedTasks = 1)
      .expect("SendService.new")

    await service.send(buildTask("budget-used", "one"))
    await service.send(buildTask("queued", "two"))

    let rejected = buildTask("rejected", "three")
    let errorFuture = newFuture[MessageErrorEvent]("rate-limit-queue-full")
    let listener = MessageErrorEvent
      .listen(
        waku.brokerCtx,
        proc(event: MessageErrorEvent) {.async: (raises: []).} =
          if event.requestId == rejected.requestId and not errorFuture.finished():
            errorFuture.complete(event)
        ,
      )
      .expect("listen MessageErrorEvent")

    await service.send(rejected)
    let received = await errorFuture.withTimeout(1.seconds)
    check:
      received
      rejected.state == DeliveryState.FailedToDeliver
      processor.calls == 1
    if received:
      let event = errorFuture.read()
      check:
        event.error == "rate limit queue is full"
        event.requestId == rejected.requestId
    await MessageErrorEvent.dropListener(waku.brokerCtx, listener)

    epoch = 2'u64
    await service.trySendMessages()
    check processor.calls == 2
