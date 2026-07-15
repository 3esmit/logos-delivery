import std/strutils
import chronos, chronicles, results, ffi
import logos_delivery, library/declare_lib

proc wakuDiscv5UpdateBootnodes*(
    lib: LogosDelivery, bootnodes: string
): Future[Result[string, string]] {.ffi.} =
  ## Updates the bootnode list used for discovering new peers via DiscoveryV5
  ## bootnodes - JSON array containing the bootnode ENRs i.e. `["enr:...", "enr:..."]`
  (await lib.waku.discv5UpdateBootnodes(bootnodes)).isOkOr:
    error "UPDATE_DISCV5_BOOTSTRAP_NODES failed", error = error
    return err(error)
  return ok("discovery request processed correctly")

proc wakuDnsDiscovery*(
    lib: LogosDelivery, enrTreeUrl: string, nameDnsServer: string, timeoutMs: int32
): Future[Result[string, string]] {.ffi.} =
  let nodes = (
    await lib.waku.dnsDiscovery(enrTreeUrl, nameDnsServer, int(timeoutMs))
  ).valueOr:
    error "GET_BOOTSTRAP_NODES failed", error = error
    return err(error)
  ## returns a comma-separated string of bootstrap nodes' multiaddresses
  return ok(nodes.join(","))

proc wakuStartDiscv5*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.startDiscv5()).isOkOr:
    error "START_DISCV5 failed", error = error
    return err(error)
  return ok("discv5 started correctly")

proc wakuStopDiscv5*(lib: LogosDelivery): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.stopDiscv5()).isOkOr:
    error "STOP_DISCV5 failed", error = error
    return err(error)
  return ok("discv5 stopped correctly")

proc wakuPeerExchangeRequest*(
    lib: LogosDelivery, numPeers: uint64
): Future[Result[string, string]] {.ffi.} =
  let numValidPeers = (await lib.waku.peerExchangeRequest(numPeers)).valueOr:
    error "waku_peer_exchange_request failed", error = error
    return err(error)
  return ok($numValidPeers)
