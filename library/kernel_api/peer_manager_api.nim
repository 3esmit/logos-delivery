import std/[strutils, tables, json]
import chronicles, chronos, results, ffi
import logos_delivery, library/declare_lib

type PeerInfo = object
  protocols: seq[string]
  addresses: seq[string]

proc wakuGetPeeridsFromPeerstore*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  ## returns a comma-separated string of peerIDs
  let peerIds = (await lib.waku.peerIdsFromPeerstore()).valueOr:
    return err(error)
  return ok(peerIds.join(","))

proc wakuConnect*(
    lib: LogosDelivery, peerMultiAddr: string, timeoutMs: uint32
): Future[Result[string, string]] {.ffi.} =
  let peers = peerMultiAddr.split(",")
  (await lib.waku.connect(peers, uint32(timeoutMs))).isOkOr:
    return err(error)
  return ok("")

proc wakuDisconnectPeerById*(
    lib: LogosDelivery, peerId: string
): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.disconnectPeerById(peerId)).isOkOr:
    error "DISCONNECT_PEER_BY_ID failed", error = error
    return err(error)
  return ok("")

proc wakuDisconnectAllPeers*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.disconnectAllPeers()).isOkOr:
    return err(error)
  return ok("")

proc wakuDialPeer*(
    lib: LogosDelivery, peerMultiAddr: string, protocol: string, timeoutMs: uint32
): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.dialPeer(peerMultiAddr, protocol, int(timeoutMs))).isOkOr:
    error "DIAL_PEER failed", error = error
    return err(error)
  return ok("")

proc wakuDialPeerById*(
    lib: LogosDelivery, peerId: string, protocol: string, timeoutMs: uint32
): Future[Result[string, string]] {.ffi.} =
  (await lib.waku.dialPeerById(peerId, protocol, int(timeoutMs))).isOkOr:
    error "DIAL_PEER_BY_ID failed", error = error
    return err(error)
  return ok("")

proc wakuGetConnectedPeersInfo*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  ## returns a JSON string mapping peerIDs to objects with protocols and addresses
  let peers = (await lib.waku.connectedPeersInfo()).valueOr:
    return err(error)

  var peersMap = initTable[string, PeerInfo]()
  for peer in peers:
    peersMap[peer.peerId] =
      PeerInfo(protocols: peer.protocols, addresses: peer.addresses)

  return ok($(%*peersMap))

proc wakuGetConnectedPeers*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  ## returns a comma-separated string of peerIDs
  let peerIds = (await lib.waku.connectedPeers()).valueOr:
    return err(error)
  return ok(peerIds.join(","))

proc wakuGetPeeridsByProtocol*(
    lib: LogosDelivery, protocol: string
): Future[Result[string, string]] {.ffi.} =
  ## returns a comma-separated string of peerIDs that mount the given protocol
  let peerIds = (await lib.waku.peerIdsByProtocol(protocol)).valueOr:
    return err(error)
  return ok(peerIds.join(","))
