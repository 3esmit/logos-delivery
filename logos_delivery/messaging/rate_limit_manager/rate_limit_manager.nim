## Rate Limit Manager for the Messaging API.
##
## Tracks messages sent per RLN epoch and rejects admission when the
## configured budget is exhausted, ensuring RLN compliance on enforcing
## relays.
##
## `admit` meters each call against `messagesPerEpoch`, rolling the epoch
## window over once `epochPeriodSec` has elapsed since `currentEpochStart`.
## The caller (SendService) owns the pending work and re-offers parked
## tasks on the next service-loop tick, so the manager keeps no queue.
##
## See: https://lip.logos.co/messaging/raw/reliable-channel-api.html

import std/times
import results, chronos

type
  RateLimitError* {.pure.} = enum
    OverBudget

  RateLimitConfig* = object
    enabled*: bool ## spec: rate limiting opt-in; SHOULD be true when RLN active
    epochPeriodSec*: int
    messagesPerEpoch*: int

  RateLimitManager* = ref object
    config*: RateLimitConfig
    currentEpochStart*: Time
    sentInCurrentEpoch*: int

const
  DefaultEpochPeriodSec* = 600
  DefaultMessagesPerEpoch* = 1

proc new*(T: type RateLimitManager, config: RateLimitConfig): T =
  return T(config: config, currentEpochStart: getTime(), sentInCurrentEpoch: 0)

proc resetEpoch*(self: RateLimitManager) =
  self.currentEpochStart = getTime()
  self.sentInCurrentEpoch = 0

proc rollEpochIfElapsed(self: RateLimitManager) =
  ## Opens a fresh budget window once the configured epoch period has
  ## passed, so `sentInCurrentEpoch` counts only the current epoch.
  if getTime() - self.currentEpochStart >=
      initDuration(seconds = self.config.epochPeriodSec):
    self.resetEpoch()

proc admit*(
    self: RateLimitManager, msg: seq[byte]
): Future[Result[void, RateLimitError]] {.async: (raises: []).} =
  ## Meters one transmission against the per-epoch budget: rolls the epoch
  ## window if elapsed, then admits and counts the send while under
  ## `messagesPerEpoch`, or returns `OverBudget` so the caller can park it.
  if not self.config.enabled:
    return ok()

  self.rollEpochIfElapsed()

  if self.sentInCurrentEpoch >= self.config.messagesPerEpoch:
    return err(RateLimitError.OverBudget)

  self.sentInCurrentEpoch.inc()
  return ok()
