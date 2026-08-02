{.used.}

import std/net
import chronos, results, testutils/unittests
import brokers/broker_context

import ../testlib/testasync

import logos_delivery/api/[types, messaging_client_api]
import logos_delivery/api/conf/messaging_conf
import logos_delivery/messaging/[messaging_client, messaging_client_lifecycle]
import logos_delivery/waku/[waku, factory/waku_conf]
import tools/confutils/cli_args

proc testConf(): WakuConf =
  var conf = MessagingClientConf()
    .toWakuNodeConf(messaging_conf.LogosDeliveryMode.Core).valueOr:
      raiseAssert error
  conf.listenAddress = parseIpAddress("0.0.0.0")
  conf.tcpPort = Port(0)
  conf.discv5UdpPort = Port(0)
  conf.clusterId = Opt.some(3'u16)
  conf.numShardsInNetwork = 1
  conf.rest = false
  return conf.toWakuConf().valueOr:
    raiseAssert error

suite "MessagingClient - lifecycle":
  asyncTest "start rolls back the send provider after a subscribe collision":
    var waku: Waku
    var client: MessagingClient
    lockNewGlobalBrokerContext:
      let brokerCtx = globalBrokerContext()
      waku = (await Waku.new(testConf())).expect("Waku.new")
      client =
        MessagingClient.new(MessagingClientConf(), waku).expect("MessagingClient.new")

      MessagingSubscribe
        .setProvider(
          brokerCtx,
          proc(_: ContentTopic): Result[void, string] =
            ok(),
        )
        .expect("setProvider MessagingSubscribe")

      let startResult = client.start()
      check:
        startResult.isErr()
        not client.started
        not MessagingSend.isProvided(brokerCtx)
        MessagingSubscribe.isProvided(brokerCtx)
        MessagingSubscribe
          .request(brokerCtx, ContentTopic("/messaging/1/lifecycle/proto"))
          .isOk()

      MessagingSubscribe.clearProvider(brokerCtx)
      client.start().expect("retry MessagingClient.start")
      check client.started
      await client.stop()

    discard await waku.stop()

  asyncTest "start rolls back all prior providers after an unsubscribe collision":
    var waku: Waku
    var client: MessagingClient
    lockNewGlobalBrokerContext:
      let brokerCtx = globalBrokerContext()
      waku = (await Waku.new(testConf())).expect("Waku.new")
      client =
        MessagingClient.new(MessagingClientConf(), waku).expect("MessagingClient.new")

      MessagingUnsubscribe
        .setProvider(
          brokerCtx,
          proc(_: ContentTopic): Result[void, string] =
            ok(),
        )
        .expect("setProvider MessagingUnsubscribe")

      let startResult = client.start()
      check:
        startResult.isErr()
        not client.started
        not MessagingSend.isProvided(brokerCtx)
        not MessagingSubscribe.isProvided(brokerCtx)
        MessagingUnsubscribe.isProvided(brokerCtx)
        MessagingUnsubscribe
          .request(brokerCtx, ContentTopic("/messaging/1/lifecycle/proto"))
          .isOk()

      MessagingUnsubscribe.clearProvider(brokerCtx)
      client.start().expect("retry MessagingClient.start")
      check client.started
      await client.stop()

    discard await waku.stop()
