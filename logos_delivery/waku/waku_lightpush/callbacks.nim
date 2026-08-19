{.push raises: [].}

import results

import ../waku_core, ../waku_relay, ./common, ../rln, ../rln/protocol_types

import std/times, libp2p/peerid, stew/byteutils

proc getNilPushHandler*(): PushMessageHandler =
  return proc(
      pubsubTopic: string, message: WakuMessage
  ): Future[WakuLightPushResult] {.async.} =
    return lightpushResultInternalError("no waku relay found")

proc getRelayPushHandler*(wakuRelay: WakuRelay): PushMessageHandler =
  return proc(
      pubsubTopic: string, message: WakuMessage
  ): Future[WakuLightPushResult] {.async.} =
    (await wakuRelay.validateMessage(pubSubTopic, message)).isOkOr:
      return lighpushErrorResult(LightPushErrorCode.INVALID_MESSAGE, $error)

    let publishedResult = (await wakuRelay.publish(pubsubTopic, message)).valueOr:
      let msgHash = computeMessageHash(pubsubTopic, message).to0xHex()
      debug "Lightpush request has not been published to any peers",
        msg_hash = msgHash, reason = $error
      return mapPubishingErrorToPushResult(error)

    return lightpushSuccessResult(publishedResult.uint32)

proc getRelayPushHandler*(wakuRelay: WakuRelay, rln: Rln): PushMessageHandler =
  ## Builds a relay handler that preserves the RLN validation cause before the
  ## generic validator chain collapses it into a string.
  return proc(
      pubsubTopic: string, message: WakuMessage
  ): Future[WakuLightPushResult] {.async.} =
    let cause = await rln.validateMessageDetailed(message)
    case cause
    of MessageValidationCause.Valid:
      discard
    of MessageValidationCause.StaleRoot:
      return lighpushErrorResult(
        LightPushErrorCode.OUT_OF_RLN_PROOF, RlnProofRefreshScheduledMsg
      )
    else:
      return lighpushErrorResult(
        LightPushErrorCode.INVALID_MESSAGE,
        RlnTerminalValidationErrorMsg & ": " & $cause,
      )

    (await wakuRelay.validateMessage(pubSubTopic, message)).isOkOr:
      return lighpushErrorResult(LightPushErrorCode.INVALID_MESSAGE, $error)

    let publishedResult = (await wakuRelay.publish(pubsubTopic, message)).valueOr:
      let msgHash = computeMessageHash(pubsubTopic, message).to0xHex()
      notice "Lightpush request has not been published to any peers",
        msg_hash = msgHash, reason = $error
      return mapPubishingErrorToPushResult(error)

    return lightpushSuccessResult(publishedResult.uint32)
