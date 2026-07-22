{.push raises: [].}

## NAT strategy configuration.
##
## Parses the `--nat` CLI value into a `NatStrategy` and maps it onto
## libp2p's `NATConfig` (`SwitchBuilder.withNAT`), which owns UPnP / NAT-PMP
## port mapping, external-IP discovery, lease refresh and announced-address
## rewriting on a per-switch basis.

import std/[net, strutils]
import chronos, chronicles, results
import
  libp2p/services/natservice,
  libp2p/services/nat/[portmapper, upnp_mapper, natpmp_mapper]

export natservice

logScope:
  topics = "nat"

type
  NatStrategyKind* = enum
    NatNone
    NatAny
    NatUpnp
    NatPmp
    NatExtIp

  NatStrategy* = object
    case kind*: NatStrategyKind
    of NatExtIp:
      extIp*: IpAddress
    else:
      discard

proc `$`*(strategy: NatStrategy): string =
  case strategy.kind
  of NatNone:
    "none"
  of NatAny:
    "any"
  of NatUpnp:
    "upnp"
  of NatPmp:
    "pmp"
  of NatExtIp:
    "extip:" & $strategy.extIp

proc parseNatStrategy*(natConf: string): Result[NatStrategy, string] =
  case natConf.toLowerAscii()
  of "any":
    ok(NatStrategy(kind: NatAny))
  of "none":
    ok(NatStrategy(kind: NatNone))
  of "upnp":
    ok(NatStrategy(kind: NatUpnp))
  of "pmp":
    ok(NatStrategy(kind: NatPmp))
  else:
    if not natConf.startsWith("extip:"):
      return err("not a valid NAT mechanism: " & natConf)
    let ip =
      try:
        parseIpAddress(natConf[6 ..^ 1])
      except ValueError:
        return err("not a valid IP address: " & natConf[6 ..^ 1])
    ok(NatStrategy(kind: NatExtIp, extIp: ip))

type FallbackPortMapper* = ref object of PortMapper
  ## `--nat any`: try UPnP first, then NAT-PMP. Candidates are probed in
  ## order at discovery time and the first one that finds an external IP
  ## becomes the active mapper; the losers are closed. Once a mapper is
  ## active it is never re-elected.
  candidates: seq[PortMapper]
  active: PortMapper

proc new*(T: typedesc[FallbackPortMapper], candidates: varargs[PortMapper]): T =
  T(candidates: @candidates)

method discover*(
    self: FallbackPortMapper, timeout: Duration
): Future[Result[IpAddress, string]] {.async: (raises: [CancelledError]), gcsafe.} =
  if not self.active.isNil():
    return await self.active.discover(timeout)

  var errors: seq[string]
  for candidate in self.candidates:
    let res = await candidate.discover(timeout)
    if res.isOk():
      self.active = candidate
      for other in self.candidates:
        if other != candidate:
          await other.close()
      self.candidates = @[]
      return res
    errors.add(res.error)
  err("all NAT port mappers failed discovery: " & errors.join("; "))

method map*(
    self: FallbackPortMapper,
    internalPort: Port,
    externalPort: Port,
    proto: MapProto,
    lease: uint32,
): Future[Result[Port, string]] {.async: (raises: [CancelledError]), gcsafe.} =
  if self.active.isNil():
    return err("no active NAT port mapper; discovery has not succeeded")
  await self.active.map(internalPort, externalPort, proto, lease)

method unmap*(
    self: FallbackPortMapper, externalPort: Port, proto: MapProto
): Future[Result[void, string]] {.async: (raises: [CancelledError]), gcsafe.} =
  if self.active.isNil():
    return err("no active NAT port mapper; discovery has not succeeded")
  await self.active.unmap(externalPort, proto)

method close*(self: FallbackPortMapper) {.async: (raises: []), gcsafe.} =
  if not self.active.isNil():
    await self.active.close()
    self.active = nil
  for candidate in self.candidates:
    await candidate.close()
  self.candidates = @[]

const NatDiscoveryTimeout = 1.seconds
  ## Bounds the gateway discovery the NATService performs during switch
  ## start. Node start awaits discovery, and miniupnpc repeats its SSDP
  ## probe rounds each waiting the full timeout, so on networks without a
  ## gateway the start stall is several times this value per mechanism
  ## (observed: 44s at libp2p's 10-second default, 20s at 3 seconds). One
  ## second keeps the worst-case stall in single digits while still giving
  ## gateways five times the 200ms window nim-eth's setupNat allowed them
  ## before the libp2p NATService migration.

proc toNatConfig*(strategy: NatStrategy): Opt[NATConfig] =
  ## The libp2p `NATConfig` for a strategy, or none when no NATService is
  ## wanted. `NatExtIp` is deliberately not mapped: the static external IP is
  ## folded into `NetConfig`/the ENR before the switch exists, and libp2p's
  ## explicit-ip address mapper would drop dns4 announced addresses.
  case strategy.kind
  of NatUpnp, NatAny:
    Opt.some(upnpConfig(discoveryTimeout = NatDiscoveryTimeout))
  of NatPmp:
    Opt.some(natPmpConfig(discoveryTimeout = NatDiscoveryTimeout))
  of NatNone, NatExtIp:
    Opt.none(NATConfig)

proc natPortMapperFactory*(strategy: NatStrategy): PortMapperFactory =
  ## For `NatAny`, overrides libp2p's default port mapper with the UPnP-then-
  ## NAT-PMP fallback; every other strategy uses the library default (nil).
  if strategy.kind != NatAny:
    return nil
  return proc(mode: PortMappingMode): Opt[PortMapper] {.gcsafe, raises: [].} =
    try:
      Opt.some(PortMapper(FallbackPortMapper.new(UpnpMapper.new(), NatPmpMapper.new())))
    except ResourceExhaustedError as e:
      error "Failed to construct fallback NAT port mapper", err = e.msg
      Opt.none(PortMapper)
