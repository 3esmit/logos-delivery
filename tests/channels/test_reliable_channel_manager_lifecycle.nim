{.used.}

import results, chronos, testutils/unittests
import brokers/broker_context

import logos_delivery/api/[types, messaging_client_api]
import logos_delivery/api/conf/channels_conf
import logos_delivery/channels/api/channel_lifecycle
import logos_delivery/channels/[reliable_channel_manager, reliable_channel]

suite "ReliableChannelManager - lifecycle":
  asyncTest "start propagates a deferred subscription failure":
    const
      channelId = ChannelId("deferred-subscription-channel")
      contentTopic = ContentTopic("/reliable-channel/1/deferred-subscription/proto")

    var manager: ReliableChannelManager
    var subscriptionRequests = 0
    lockNewGlobalBrokerContext:
      let brokerCtx = globalBrokerContext()
      manager = ReliableChannelManager.new(ReliableChannelManagerConf()).expect(
          "ReliableChannelManager.new"
        )
      discard manager
        .createReliableChannel(channelId, contentTopic, SdsParticipantID("local"))
        .expect("createReliableChannel")

      MessagingSubscribe
        .setProvider(
          brokerCtx,
          proc(_: ContentTopic): Result[void, string] =
            inc subscriptionRequests
            err("deferred subscription failed"),
        )
        .expect("setProvider MessagingSubscribe")

      let startResult = manager.start()
      check:
        startResult.isErr()
        startResult.error ==
          "failed to subscribe channel's content topic: deferred subscription failed"
        subscriptionRequests == 1

      MessagingSubscribe.clearProvider(brokerCtx)
      await manager.stop()
