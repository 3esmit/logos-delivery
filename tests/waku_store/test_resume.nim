{.used.}

import std/net, testutils/unittests, chronos, results

import
  logos_delivery/waku/[
    node/peer_manager,
    waku_node,
    waku_core,
    waku_store/resume,
    waku_store/common,
    waku_archive/driver,
  ],
  ../testlib/[wakucore, testasync, wakunode],
  ./store_utils,
  ../waku_archive/archive_utils

suite "Store Resume":
  var resume {.threadvar.}: StoreResume

  asyncSetup:
    let resumeRes: Result[StoreResume, string] =
      StoreResume.new(peerManager = nil, wakuArchive = nil, wakuStoreClient = nil)

    assert resumeRes.isOk(), $resumeRes.error

    resume = resumeRes.get()

  asyncTeardown:
    await resume.stopWait()

  asyncTest "get set roundtrip":
    let ts = getNowInNanosecondTime()

    let setRes = resume.setLastOnlineTimestamp(ts)
    assert setRes.isOk(), $setRes.error

    let getRes = resume.getLastOnlineTimestamp()
    assert getRes.isOk(), $getRes.error

    let getTs = getRes.get()

    assert getTs == ts, "wrong timestamp"

suite "Store Resume - End to End":
  var server {.threadvar.}: WakuNode
  var client {.threadvar.}: WakuNode

  var serverDriver {.threadvar.}: ArchiveDriver
  var clientDriver {.threadvar.}: ArchiveDriver

  asyncSetup:
    let messages = @[
      fakeWakuMessage(@[byte 00]),
      fakeWakuMessage(@[byte 01]),
      fakeWakuMessage(@[byte 02]),
      fakeWakuMessage(@[byte 03]),
      fakeWakuMessage(@[byte 04]),
      fakeWakuMessage(@[byte 05]),
      fakeWakuMessage(@[byte 06]),
      fakeWakuMessage(@[byte 07]),
      fakeWakuMessage(@[byte 08]),
      fakeWakuMessage(@[byte 09]),
    ]

    let
      serverKey = generateSecp256k1Key()
      clientKey = generateSecp256k1Key()

    server = newTestWakuNode(serverKey, IPv4_any(), Port(0))
    client = newTestWakuNode(clientKey, IPv4_any(), Port(0))

    serverDriver = newArchiveDriverWithMessages(DefaultPubsubTopic, messages)
    clientDriver = newSqliteArchiveDriver()

    let mountServerArchiveRes = server.mountArchive(serverDriver)
    let mountClientArchiveRes = client.mountArchive(clientDriver)

    assert mountServerArchiveRes.isOk()
    assert mountClientArchiveRes.isOk()

    await server.mountStore()
    await client.mountStore()

    client.mountStoreClient()
    server.mountStoreClient()

    client.setupStoreResume()

    await server.start()

    let serverRemotePeerInfo = server.peerInfo.toRemotePeerInfo()

    client.peerManager.addServicePeer(serverRemotePeerInfo, WakuStoreCodec)

  asyncTeardown:
    await allFutures(client.stop(), server.stop())

  asyncTest "10 messages resume":
    var countRes = await clientDriver.getMessagesCount()
    assert countRes.isOk(), $countRes.error

    check:
      countRes.get() == 0

    await client.start()

    # catch-up runs in the background (store is a startup-only dependency;
    # node startup no longer blocks on it), so poll for its completion
    var count = 0'i64
    for _ in 0 ..< 75:
      await sleepAsync(200.milliseconds)
      count = (await clientDriver.getMessagesCount()).valueOr:
        continue
      if count == 10:
        break

    check:
      count == 10

  asyncTest "resume archives messages older than the archive freshness window":
    ## Catch-up messages are older than the archive's 20 s live-traffic
    ## freshness filter by definition, so resume must feed them through the
    ## sync ingress; through the validated live path the whole catch-up
    ## would be silently dropped.
    let hourAgo = Timestamp(getNowInNanosecondTime() - 3_600_000_000_000)
    let oldMessages = @[
      fakeWakuMessage(@[byte 10], ts = hourAgo),
      fakeWakuMessage(@[byte 11], ts = hourAgo + 1),
      fakeWakuMessage(@[byte 12], ts = hourAgo + 2),
      fakeWakuMessage(@[byte 13], ts = hourAgo + 3),
      fakeWakuMessage(@[byte 14], ts = hourAgo + 4),
    ]
    serverDriver = serverDriver.put(DefaultPubsubTopic, oldMessages)

    await client.start()

    # resume from a deep, beyond-freshness-window gap: the server holds
    # 10 fresh messages plus 5 from an hour ago, all inside the gap
    let twoHoursAgo = Timestamp(getNowInNanosecondTime() - 7_200_000_000_000)
    let serverPeer = server.peerInfo.toRemotePeerInfo()

    let res = await client.wakuStoreResume.startStoreResume(twoHoursAgo, serverPeer)
    assert res.isOk(), $res.error

    let countRes = await clientDriver.getMessagesCount()
    assert countRes.isOk(), $countRes.error

    check:
      countRes.get() == 15
