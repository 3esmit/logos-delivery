{.used.}

import chronos, testutils/unittests, stew/byteutils

import logos_delivery/messaging/rate_limit_manager/rate_limit_manager

suite "RateLimitManager - admission":
  asyncTest "admit is a pass-through when disabled":
    let rl = RateLimitManager.new(
      RateLimitConfig(enabled: false, epochPeriodSec: 600, messagesPerEpoch: 1)
    )
    for _ in 0 ..< 10:
      let res = await rl.admit("payload".toBytes())
      check res.isOk()

  asyncTest "admit enforces the per-epoch budget when enabled":
    let rl = RateLimitManager.new(
      RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1)
    )
    check (await rl.admit("first".toBytes())).isOk()

    let overBudget = await rl.admit("second".toBytes())
    check overBudget.isErr()
    check overBudget.error == RateLimitError.OverBudget

  asyncTest "admit reopens the budget after the epoch rolls over":
    let rl = RateLimitManager.new(
      RateLimitConfig(enabled: true, epochPeriodSec: 600, messagesPerEpoch: 1)
    )
    check (await rl.admit("first".toBytes())).isOk()
    check (await rl.admit("second".toBytes())).isErr()

    # Simulate the epoch window elapsing; the next call must be admitted again.
    rl.resetEpoch()
    check (await rl.admit("third".toBytes())).isOk()
