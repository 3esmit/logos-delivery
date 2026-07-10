{.used.}

## Regression suite: relay peers present in the peer store at startup must not
## trigger reconnect-with-backoff behavior.
##
## Fast peer discovery (e.g. Kademlia service discovery bootstrapping) can
## populate the peer store before or during node startup. The peer store alone
## cannot tell such freshly discovered peers apart from peers restored from
## persistent storage, so any startup logic keyed on "the store has relay
## peers" misfires: discovered peers were being swept into a
## reconnect-after-prune-backoff wait (~62s), applied serially per peer, which
## stalled connectivity for minutes and blocked late relay mounts.
##
## The contract these tests pin down:
## - Adding a peer to the peer store is bookkeeping, not a dial instruction;
##   neither node.start() nor a late mountRelay() may dial or sleep on store
##   contents.
## - Establishing and re-establishing relay connectivity is the connectivity
##   maintenance path's job (connectToRelayPeers / relayConnectivityLoop),
##   which is prompt and respects per-peer failure backoff.

import testutils/unittests, chronos, libp2p/[peerid, multiaddress]

import
  logos_delivery/waku/[waku_core, waku_node, node/peer_manager, waku_relay/protocol],
  ../testlib/[wakucore, wakunode, testasync, testutils]

# Valid peerId missing the last digit; append a digit to mint distinct peers.
const BasePeerId = "QmeuZJbXrszW2jdT7GdduSjQskPU3S7vvGWKtKgDfkDvW"

proc unreachableRelayPeer(n: int, origin = PeerOrigin.Kademlia): RemotePeerInfo =
  ## A relay-capable peer record the way discovery would hand it over:
  ## peer id, dialable-looking address, advertised relay protocol. The port
  ## has no listener, so any dial attempt fails fast and leaves a trace in
  ## numberFailedConn.
  var peerId: PeerId
  doAssert peerId.init(BasePeerId & $n)
  RemotePeerInfo.init(
    peerId = peerId,
    addrs = @[MultiAddress.init("/ip4/127.0.0.1/tcp/" & $n).tryGet()],
    protocols = @[WakuRelayCodec],
    origin = origin,
  )

suite "Startup relay reconnect regression":
  asyncTest "mounting relay on a running node is not stalled by known relay peers":
    ## Given a started node without relay, whose peer store discovery has
    ## already populated with relay-capable peers
    let node = newTestWakuNode(generateSecp256k1Key())
    await node.start()
    defer:
      await node.stop()

    for n in 1 .. 3:
      node.peerManager.addPeer(unreachableRelayPeer(n), PeerOrigin.Kademlia)

    ## When relay is mounted late (e.g. edge node promoted to relay)
    let mountFut = node.mountRelay()
    let mountedInTime = await mountFut.withTimeout(10.seconds)
    if not mountedInTime:
      await mountFut.cancelAndWait()

    ## Then the mount completes promptly instead of sleeping a prune backoff
    ## per known peer
    check mountedInTime
    if mountedInTime:
      check mountFut.read().isOk()

  asyncTest "already-connected relay peers do not stall a late relay mount":
    ## Given a relay server and a started relay-less client connected to it
    let
      server = newTestWakuNode(generateSecp256k1Key())
      client = newTestWakuNode(generateSecp256k1Key())
    (await server.mountRelay()).isOkOr:
      raiseAssert "mountRelay server: " & error
    await allFutures(server.start(), client.start())
    defer:
      await allFutures(client.stop(), server.stop())

    let
      serverPeerInfo = server.switch.peerInfo.toRemotePeerInfo()
      clientPeerStore = client.peerManager.switch.peerStore
    await client.connectToNodes(@[serverPeerInfo])

    # identify has advertised the server's relay support to the client
    waitActive:
      clientPeerStore.getPeer(serverPeerInfo.peerId).protocols.contains(WakuRelayCodec)

    ## When the client mounts relay while that relay-capable peer is connected
    let mountFut = client.mountRelay()
    let mountedInTime = await mountFut.withTimeout(10.seconds)
    if not mountedInTime:
      await mountFut.cancelAndWait()

    ## Then the mount is not delayed on account of the connected peer
    check mountedInTime
    if mountedInTime:
      check mountFut.read().isOk()
    check clientPeerStore.getPeer(serverPeerInfo.peerId).connectedness == Connected

  asyncTest "startup does not auto-dial peers that discovery added to the peer store":
    ## Given a relay server and a relay client that learned about the server
    ## (plus some unreachable peers) via discovery before starting
    let
      server = newTestWakuNode(generateSecp256k1Key())
      client = newTestWakuNode(generateSecp256k1Key())
    (await server.mountRelay()).isOkOr:
      raiseAssert "mountRelay server: " & error
    (await client.mountRelay()).isOkOr:
      raiseAssert "mountRelay client: " & error
    await server.start()

    # keep any legacy reconnect wait short enough to observe inside the window
    client.wakuRelay.parameters.pruneBackoff = chronos.seconds(1)

    let
      serverPeerInfo = server.switch.peerInfo.toRemotePeerInfo()
      clientPeerStore = client.peerManager.switch.peerStore
    client.peerManager.addPeer(serverPeerInfo, PeerOrigin.Kademlia)
    for n in 1 .. 2:
      client.peerManager.addPeer(unreachableRelayPeer(n), PeerOrigin.Kademlia)

    ## When the client starts
    await client.start()
    defer:
      await allFutures(client.stop(), server.stop())

    ## Then no store peer is dialed behind the connectivity policy's back
    var autoDialed = false
    for _ in 0 ..< 16:
      if clientPeerStore.getPeer(serverPeerInfo.peerId).connectedness == Connected:
        autoDialed = true
        break
      await sleepAsync(500.milliseconds)

    check not autoDialed
    for n in 1 .. 2:
      check clientPeerStore.getPeer(unreachableRelayPeer(n).peerId).numberFailedConn == 0

  asyncTest "startup ignores peer-store relay peers regardless of peer origin":
    ## Given a relay node whose peer store holds relay-capable peers of
    ## assorted origins (none of them from persistent storage — persistence
    ## is not even enabled)
    let node = newTestWakuNode(generateSecp256k1Key())
    (await node.mountRelay()).isOkOr:
      raiseAssert "mountRelay: " & error
    node.wakuRelay.parameters.pruneBackoff = chronos.seconds(1)

    let peerStore = node.peerManager.switch.peerStore
    let origins =
      [PeerOrigin.Kademlia, PeerOrigin.Discv5, PeerOrigin.Static, PeerOrigin.Dns]
    for n, origin in origins:
      node.peerManager.addPeer(unreachableRelayPeer(n + 1, origin), origin)

    ## When the node starts and runs for a while
    await node.start()
    defer:
      await node.stop()
    await sleepAsync(6.seconds)

    ## Then none of them was treated as a reconnect candidate
    for n, origin in origins:
      let peer = peerStore.getPeer(unreachableRelayPeer(n + 1, origin).peerId)
      check:
        peer.numberFailedConn == 0
        peer.connectedness == NotConnected

  asyncTest "the connectivity loop connects discovered relay peers promptly":
    ## Given a relay server known to the client only through its peer store
    ## (as after discovery), with the connectivity maintenance loop running
    ## as node_factory.startNode wires it in production
    let
      server = newTestWakuNode(generateSecp256k1Key())
      client = newTestWakuNode(generateSecp256k1Key())
    (await server.mountRelay()).isOkOr:
      raiseAssert "mountRelay server: " & error
    (await client.mountRelay()).isOkOr:
      raiseAssert "mountRelay client: " & error
    await allFutures(server.start(), client.start())
    defer:
      await allFutures(client.stop(), server.stop())

    let
      serverPeerInfo = server.switch.peerInfo.toRemotePeerInfo()
      clientPeerStore = client.peerManager.switch.peerStore
    client.peerManager.addPeer(serverPeerInfo, PeerOrigin.Kademlia)

    ## When the connectivity loop starts
    client.peerManager.start()
    defer:
      client.peerManager.stop()

    ## Then the discovered peer is connected promptly, no backoff involved
    waitActive:
      clientPeerStore.getPeer(serverPeerInfo.peerId).connectedness == Connected

  asyncTest "relay connectivity is re-established on demand via the maintenance path":
    ## Given two connected relay nodes that then disconnect
    let
      server = newTestWakuNode(generateSecp256k1Key())
      client = newTestWakuNode(generateSecp256k1Key())
    (await server.mountRelay()).isOkOr:
      raiseAssert "mountRelay server: " & error
    (await client.mountRelay()).isOkOr:
      raiseAssert "mountRelay client: " & error
    await allFutures(server.start(), client.start())
    defer:
      await allFutures(client.stop(), server.stop())

    let
      serverPeerInfo = server.switch.peerInfo.toRemotePeerInfo()
      clientPeerStore = client.peerManager.switch.peerStore
    await client.connectToNodes(@[serverPeerInfo])
    waitActive:
      clientPeerStore.getPeer(serverPeerInfo.peerId).connectedness == Connected

    await client.disconnectNode(serverPeerInfo)
    waitActive:
      clientPeerStore.getPeer(serverPeerInfo.peerId).connectedness == CanConnect

    ## When the connectivity maintenance pass runs
    await client.peerManager.connectToRelayPeers()

    ## Then the relay peer is connected again, with no built-in delay
    waitActive:
      clientPeerStore.getPeer(serverPeerInfo.peerId).connectedness == Connected

  asyncTest "restart re-establishes relay connectivity via the maintenance path":
    ## Given a relay node that was connected to a relay peer and restarted
    let
      node1 = newTestWakuNode(generateSecp256k1Key())
      node2 = newTestWakuNode(generateSecp256k1Key())
    (await node1.mountRelay()).isOkOr:
      raiseAssert "mountRelay node1: " & error
    (await node2.mountRelay()).isOkOr:
      raiseAssert "mountRelay node2: " & error
    await allFutures(node1.start(), node2.start())

    let
      node2PeerInfo = node2.switch.peerInfo.toRemotePeerInfo()
      node1PeerStore = node1.peerManager.switch.peerStore
    await node1.connectToNodes(@[node2PeerInfo])
    waitActive:
      node1PeerStore.getPeer(node2PeerInfo.peerId).connectedness == Connected

    await node1.stop()

    ## When it starts again and the connectivity maintenance pass runs
    let restarted = await node1.start().withTimeout(10.seconds)
    check restarted
    defer:
      await allFutures(node1.stop(), node2.stop())

    await node1.peerManager.connectToRelayPeers()

    ## Then the previously-known relay peer is connected again promptly
    waitActive:
      node1PeerStore.getPeer(node2PeerInfo.peerId).connectedness == Connected

  asyncTest "start and stop with a populated peer store are prompt and clean":
    ## Given a relay node whose peer store holds unreachable relay peers
    let node = newTestWakuNode(generateSecp256k1Key())
    (await node.mountRelay()).isOkOr:
      raiseAssert "mountRelay: " & error
    for n in 1 .. 3:
      node.peerManager.addPeer(unreachableRelayPeer(n), PeerOrigin.Kademlia)

    ## When it starts and immediately stops
    let startedInTime = await node.start().withTimeout(10.seconds)
    check startedInTime

    let stoppedInTime = await node.stop().withTimeout(10.seconds)

    ## Then neither direction blocks on peer-store contents
    check stoppedInTime

  asyncTest "relay streams form normally after startup with a pre-populated peer store":
    ## Given a relay client that starts with discovered peers (reachable and
    ## not) already in its peer store
    let
      server = newTestWakuNode(generateSecp256k1Key())
      client = newTestWakuNode(generateSecp256k1Key())
    (await server.mountRelay()).isOkOr:
      raiseAssert "mountRelay server: " & error
    (await client.mountRelay()).isOkOr:
      raiseAssert "mountRelay client: " & error
    await server.start()

    for n in 1 .. 2:
      client.peerManager.addPeer(unreachableRelayPeer(n), PeerOrigin.Kademlia)
    await client.start()
    defer:
      await allFutures(client.stop(), server.stop())

    ## When it connects to a reachable relay peer
    await client.connectToNodes(@[server.switch.peerInfo.toRemotePeerInfo()])

    ## Then relay protocol streams are established as usual
    waitActive:
      client.peerManager.connectedPeers(WakuRelayCodec)[1].len == 1
