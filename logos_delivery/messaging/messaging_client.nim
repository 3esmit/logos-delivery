## Messaging layer core: the `MessagingClient` type plus its construction and
## lifecycle. The public operations (subscribe / unsubscribe / send) live in
## `messaging/api.nim`.
import results, chronos, chronicles
import
  logos_delivery/api/conf/messaging_conf,
  logos_delivery/api/messaging_client_api,
  logos_delivery/waku/waku,
  logos_delivery/waku/api/publish,
  logos_delivery/waku/factory/conf_builder/waku_conf_builder,
  logos_delivery/messaging/delivery_service/[recv_service, send_service],
  logos_delivery/messaging/rate_limit_manager/rate_limit_manager

export messaging_client_api, messaging_conf

type MessagingClient* = ref object
  brokerCtx*: BrokerContext
  waku*: Waku ## The Waku kernel this layer drives; read by `messaging/api/*`.
  sendService*: SendService
  recvService*: RecvService
  started*: bool
  ## The REST router is owned by Waku and persists across node restarts.
  ## Avoid registering the same MessagingClient routes more than once.
  restApiMounted: bool

proc rlnQuotaProvider(waku: Waku): QuotaProvider =
  ## Sources the rate limit manager's epoch and limit from RLN. The closure
  ## queries `waku` on each admission, so a node whose RLN mounts after
  ## construction upgrades from the wall-clock fallback automatically.
  return proc(): Opt[EpochQuota] {.gcsafe, raises: [].} =
    let q = waku.currentRlnEpochQuota().valueOr:
      return Opt.none(EpochQuota)
    return
      Opt.some(EpochQuota(epochIndex: q.epochIndex, userMessageLimit: q.messageLimit))

proc new*(
    T: type MessagingClient, conf: MessagingClientConf, waku: Waku
): Result[T, string] =
  ## The messaging layer chains onto Waku: it drives the underlying Waku kernel
  ## for transport while exposing its own send/recv API.
  let reliability = conf.reliabilityEnabled.get(DefaultP2pReliability)
  let rateLimitManager = ?RateLimitManager.new(
    conf.rateLimit.get(DefaultRateLimitConfig), rlnQuotaProvider(waku)
  )
  let sendService = ?SendService.new(reliability, waku, rateLimitManager)
  let recvService = RecvService.new(waku)
  return ok(
    T(
      waku: waku,
      sendService: sendService,
      recvService: recvService,
      brokerCtx: waku.brokerCtx,
    )
  )

proc needsRestApiMount*(self: MessagingClient): bool =
  ## The REST integration layer uses this to avoid duplicate router entries
  ## after a MessagingClient stop/start cycle.
  not self.restApiMounted

proc markRestApiMounted*(self: MessagingClient) =
  ## Record successful REST router setup. The router stays alive until Waku is
  ## destroyed, even while its HTTP listener is stopped.
  self.restApiMounted = true

proc checkApiAvailability*(self: MessagingClient): Result[void, string] =
  ## Shared guard for the api operation module.
  if self.isNil():
    return err("MessagingClient is not initialized")

  return ok()
