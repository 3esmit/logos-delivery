{.used.}

import std/[net, strutils]
import testutils/unittests, chronos, results
import libp2p/services/nat/portmapper
import ../../logos_delivery/waku/common/nat_config

suite "NAT config - strategy parsing":
  test "valid strategies parse":
    check:
      parseNatStrategy("any").get().kind == NatAny
      parseNatStrategy("none").get().kind == NatNone
      parseNatStrategy("upnp").get().kind == NatUpnp
      parseNatStrategy("pmp").get().kind == NatPmp

  test "parsing is case-insensitive":
    check:
      parseNatStrategy("UPnP").get().kind == NatUpnp
      parseNatStrategy("NONE").get().kind == NatNone

  test "extip carries the address":
    let strategy = parseNatStrategy("extip:203.0.113.7").get()
    check:
      strategy.kind == NatExtIp
      $strategy.extIp == "203.0.113.7"

  test "invalid mechanism is rejected":
    check:
      parseNatStrategy("bogus").isErr()
      parseNatStrategy("").isErr()

  test "invalid extip address is rejected":
    check:
      parseNatStrategy("extip:notanip").isErr()

  test "strategies render back to their config strings":
    check:
      $parseNatStrategy("any").get() == "any"
      $parseNatStrategy("extip:203.0.113.7").get() == "extip:203.0.113.7"

suite "NAT config - NATConfig mapping":
  test "upnp, any and pmp produce a NATConfig":
    check:
      toNatConfig(NatStrategy(kind: NatUpnp)).isSome()
      toNatConfig(NatStrategy(kind: NatAny)).isSome()
      toNatConfig(NatStrategy(kind: NatPmp)).isSome()

  test "none and extip produce no NATConfig":
    check:
      toNatConfig(NatStrategy(kind: NatNone)).isNone()
      toNatConfig(NatStrategy(kind: NatExtIp, extIp: parseIpAddress("203.0.113.7")))
        .isNone()

  test "only the any strategy overrides the port mapper factory":
    check:
      natPortMapperFactory(NatStrategy(kind: NatUpnp)).isNil()
      natPortMapperFactory(NatStrategy(kind: NatPmp)).isNil()
      natPortMapperFactory(NatStrategy(kind: NatNone)).isNil()
      not natPortMapperFactory(NatStrategy(kind: NatAny)).isNil()

type StubMapper = ref object of PortMapper
  ## Scripted port mapper: discovery yields `ip` when set, an error otherwise.
  ip: Opt[IpAddress]
  discoverCalls: int
  mapCalls: int
  unmapCalls: int
  closeCalls: int

method discover(
    self: StubMapper, timeout: Duration
): Future[Result[IpAddress, string]] {.async: (raises: [CancelledError]), gcsafe.} =
  inc self.discoverCalls
  let ip = self.ip.valueOr:
    return err("stub discovery failure")
  return ok(ip)

method map(
    self: StubMapper,
    internalPort: Port,
    externalPort: Port,
    proto: MapProto,
    lease: uint32,
): Future[Result[Port, string]] {.async: (raises: [CancelledError]), gcsafe.} =
  inc self.mapCalls
  return ok(externalPort)

method unmap(
    self: StubMapper, externalPort: Port, proto: MapProto
): Future[Result[void, string]] {.async: (raises: [CancelledError]), gcsafe.} =
  inc self.unmapCalls
  return ok()

method close(self: StubMapper) {.async: (raises: []), gcsafe.} =
  inc self.closeCalls

proc stub(ip = ""): StubMapper =
  let address =
    if ip.len > 0:
      Opt.some(parseIpAddress(ip))
    else:
      Opt.none(IpAddress)
  StubMapper(ip: address)

suite "NAT config - fallback port mapper":
  asyncTest "first successful candidate is elected and the rest are closed":
    let
      first = stub("203.0.113.1")
      second = stub("203.0.113.2")
      mapper = FallbackPortMapper.new(first, second)

    let res = await mapper.discover(1.seconds)
    check:
      $res.get() == "203.0.113.1"
      second.discoverCalls == 0 # never probed, first already won
      second.closeCalls == 1 # loser is closed
      first.closeCalls == 0

  asyncTest "discovery falls back to the next candidate":
    let
      first = stub()
      second = stub("203.0.113.2")
      mapper = FallbackPortMapper.new(first, second)

    let res = await mapper.discover(1.seconds)
    check:
      $res.get() == "203.0.113.2"
      first.discoverCalls == 1
      first.closeCalls == 1

  asyncTest "an elected mapper is never re-elected":
    let
      first = stub("203.0.113.1")
      second = stub("203.0.113.2")
      mapper = FallbackPortMapper.new(first, second)

    discard await mapper.discover(1.seconds)
    discard await mapper.discover(1.seconds)
    check:
      first.discoverCalls == 2 # re-discovery delegates to the active mapper
      second.discoverCalls == 0

  asyncTest "discovery reports every candidate failure":
    let mapper = FallbackPortMapper.new(stub(), stub())

    let res = await mapper.discover(1.seconds)
    check:
      res.isErr()
      "all NAT port mappers failed discovery" in res.error
      res.error.count("stub discovery failure") == 2

  asyncTest "map and unmap require a successful discovery":
    let mapper = FallbackPortMapper.new(stub("203.0.113.1"))

    check:
      (await mapper.map(Port(1), Port(1), mpTcp, 60)).isErr()
      (await mapper.unmap(Port(1), mpTcp)).isErr()

  asyncTest "map and unmap delegate to the active mapper":
    let
      active = stub("203.0.113.1")
      mapper = FallbackPortMapper.new(active)

    discard await mapper.discover(1.seconds)
    check:
      (await mapper.map(Port(7), Port(9), mpTcp, 60)).get() == Port(9)
      (await mapper.unmap(Port(9), mpTcp)).isOk()
      active.mapCalls == 1
      active.unmapCalls == 1

  asyncTest "close closes the active mapper once":
    let
      active = stub("203.0.113.1")
      loser = stub("203.0.113.2")
      mapper = FallbackPortMapper.new(active, loser)

    discard await mapper.discover(1.seconds)
    await mapper.close()
    await mapper.close()
    check:
      active.closeCalls == 1
      loser.closeCalls == 1

  asyncTest "close before discovery closes every candidate":
    let
      first = stub("203.0.113.1")
      second = stub("203.0.113.2")
      mapper = FallbackPortMapper.new(first, second)

    await mapper.close()
    check:
      first.closeCalls == 1
      second.closeCalls == 1
