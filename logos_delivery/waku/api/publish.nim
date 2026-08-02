## Waku layer API — message publish primitives used by the messaging send
## pipeline.
##
## Unlike `relay.nim`/`lightpush.nim`, these preserve the rich
## `WakuLightPushResult` (status code + description) that the send processors
## branch on for their retry decisions, and expose relay/lightpush availability
## so the messaging layer never inspects `waku.node` directly.
{.push raises: [].}

import std/[times, strutils]
import results, chronos

import logos_delivery/waku/waku
import
  logos_delivery/waku/[
    waku_core,
    node/waku_node,
    node/waku_node/lightpush,
    node/peer_manager,
    waku_relay/protocol,
    rln,
    waku_lightpush/common,
    waku_lightpush/rpc,
    waku_lightpush/client,
    waku_lightpush/callbacks,
  ]

# WakuLightPushResult, PushMessageHandler, LightPushErrorCode (common) plus the
# LightPushStatusCode `$`/`==` the send processors branch on (rpc).
export common, rpc

proc hasRelay*(self: Waku): bool =
  ## True if relay (gossipsub publishing) is mounted.
  return not self.node.wakuRelay.isNil()

proc hasLightpush*(self: Waku): bool =
  ## True if a lightpush client is mounted.
  return not self.node.wakuLightpushClient.isNil()

proc relayPushHandler*(self: Waku): PushMessageHandler =
  ## Builds the relay publish handler used by the send pipeline. Caller
  ## ensures relay is mounted. The handler validates and republishes; the
  ## proof is attached by the messaging layer via `attachRlnProof`.
  return getRelayPushHandler(self.node.wakuRelay)

proc attachRlnProof*(
    self: Waku, message: WakuMessage
): Future[Result[WakuMessage, string]] {.async.} =
  ## A proof-bearing message is returned unchanged so an ordinary delivery
  ## retry reuses its nonce. Without RLN mounted the message passes through.
  if self.node.rln.isNil() or message.proof.len > 0:
    return ok(message)

  var msgWithProof = message
  msgWithProof.proof = (
    await self.node.rln.generateRLNProofWithRootRefresh(
      message.toRLNSignal(), float64(getTime().toUnix())
    )
  ).valueOr:
    return err("failed to attach RLN proof: " & error)

  return ok(msgWithProof)

func isStaleRlnProof*(error: ErrorStatus): bool =
  ## A lightpush result normalized by the node carries the explicit refresh
  ## marker. Relay validates locally and returns a raw validator marker, so
  ## recognize that narrow INVALID_MESSAGE form too. That relay marker is not
  ## root-specific—relay collapses validator causes—but send-service-generated
  ## proofs treat it as a stale-proof suspicion and refresh before retrying.
  ## Other RLN and invalid message failures retain their normal retry/error
  ## handling.
  return
    (
      error.code == LightPushErrorCode.OUT_OF_RLN_PROOF and
      error.desc.get("").contains(RlnProofRefreshScheduledMsg)
    ) or (
      error.code == LightPushErrorCode.INVALID_MESSAGE and
      error.desc.get("").contains(RlnValidatorErrorMsg)
    )

proc onRlnProofRejected*(self: Waku) =
  ## Start a non-blocking path refresh; the send-service loop owns retrying.
  if self.node.rln.isNil():
    return

  self.node.rln.groupManager.scheduleMerkleProofRefresh()

proc currentRlnEpochQuota*(self: Waku): Opt[tuple[epochIndex, messageLimit: uint64]] =
  ## RLN's current epoch index and user message limit, read together so the
  ## pair cannot straddle an epoch boundary.
  if self.node.rln.isNil():
    return Opt.none(tuple[epochIndex, messageLimit: uint64])

  let limit = self.node.rln.groupManager.userMessageLimit.valueOr:
    return Opt.none(tuple[epochIndex, messageLimit: uint64])

  return Opt.some((fromEpoch(self.node.rln.getCurrentEpoch()), uint64(limit)))

proc lightpushPeerAvailable*(self: Waku, shard: PubsubTopic): bool =
  ## True if a lightpush service peer is available for `shard`.
  return self.node.peerManager.selectPeer(WakuLightPushCodec, Opt.some(shard)).isSome()

proc lightpushPublishToAny*(
    self: Waku, shard: PubsubTopic, message: WakuMessage
): Future[WakuLightPushResult] {.async.} =
  ## Selects a lightpush service peer for `shard` and publishes `message`
  ## through the node's lightpush flow. The send service normally supplies the
  ## proof, while the node preserves that proof unchanged. Returns
  ## SERVICE_NOT_AVAILABLE when no peer is available.
  let peer = self.node.peerManager.selectPeer(WakuLightPushCodec, Opt.some(shard)).valueOr:
    return lightpushResultServiceUnavailable("no lightpush peer available for shard")
  try:
    return await self.node.lightpushPublish(Opt.some(shard), message, Opt.some(peer))
  except CatchableError as e:
    return lightpushResultInternalError(e.msg)
