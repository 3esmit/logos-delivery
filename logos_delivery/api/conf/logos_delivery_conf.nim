{.push raises: [].}

import std/options
import results

import logos_delivery/api/conf/messaging_conf
import logos_delivery/api/conf/channels_conf

export options, messaging_conf, channels_conf

type LogosDeliveryMode* {.pure.} = enum
  Edge # client-only node
  Core # full service node
  Fleet # kernel-only node from a raw kernel config

proc toMessagingMode*(mode: LogosDeliveryMode): ConfResult[MessagingMode] =
  ## Fleet mounts no messaging layer, so it has no messaging mode.
  case mode
  of LogosDeliveryMode.Edge:
    return ok(MessagingMode.Edge)
  of LogosDeliveryMode.Core:
    return ok(MessagingMode.Core)
  of LogosDeliveryMode.Fleet:
    return
      err("fleet mode takes a raw kernel config; use LogosDelivery.new(kernelConf)")

type LogosDeliveryConf* = object
  ## Aggregates the per-layer config objects. A layer is mounted iff its config
  ## is present.
  kernelConf*: KernelConf
  messagingConf*: Option[MessagingClientConf]
  channelsConf*: Option[ReliableChannelManagerConf]

proc init*(T: type LogosDeliveryConf, kernelConf: KernelConf): LogosDeliveryConf =
  return LogosDeliveryConf(kernelConf: kernelConf)

proc init*(
    T: type LogosDeliveryConf,
    mode: LogosDeliveryMode,
    preset: string,
    messagingOverrides: MessagingClientConf,
    channelsOverrides: ReliableChannelManagerConf,
): ConfResult[LogosDeliveryConf] =
  let merged = merge(?resolvePreset(preset), messagingOverrides)
  var kernelConf = ?toWakuNodeConf(merged, ?toMessagingMode(mode))
  kernelConf.preset = preset
  return ok(
    LogosDeliveryConf(
      kernelConf: KernelConf(kernelConf),
      messagingConf: some(merged),
      channelsConf: some(channelsOverrides),
    )
  )

{.pop.}
