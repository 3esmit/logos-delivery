import std/strutils
import chronos, results, ffi
import logos_delivery, library/declare_lib

proc wakuVersion*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  let v = (await lib.waku.version()).valueOr:
    return err(error)
  return ok(v)

proc wakuListenAddresses*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  ## returns a comma-separated string of the listen addresses
  let addrs = (await lib.waku.listenAddresses()).valueOr:
    return err(error)
  return ok(addrs.join(","))

proc wakuGetMyEnr*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  let enrUri = (await lib.waku.myEnr()).valueOr:
    return err(error)
  return ok(enrUri)

proc wakuGetMyPeerid*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  let peerId = (await lib.waku.myPeerId()).valueOr:
    return err(error)
  return ok(peerId)

proc wakuGetMetrics*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  let m = (await lib.waku.metrics()).valueOr:
    return err(error)
  return ok(m)

proc wakuIsOnline*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  let online = (await lib.waku.isOnline()).valueOr:
    return err(error)
  return ok($online)
