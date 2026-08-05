{.used.}

import std/tables
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

      MessagingSubscribeChannel
        .setProvider(
          brokerCtx,
          proc(_: ContentTopic): Result[void, string] =
            inc subscriptionRequests
            err("deferred subscription failed"),
        )
        .expect("setProvider MessagingSubscribeChannel")

      let startResult = manager.start()
      check:
        startResult.isErr()
        startResult.error ==
          "failed to subscribe channel's content topic: deferred subscription failed"
        subscriptionRequests == 1

      MessagingSubscribeChannel.clearProvider(brokerCtx)
      await manager.stop()

  asyncTest "start rolls back earlier deferred channel leases when a later lease fails":
    const
      firstChannelId = ChannelId("deferred-first")
      firstTopic = ContentTopic("/reliable-channel/1/deferred-first/proto")
      secondChannelId = ChannelId("deferred-second")
      secondTopic = ContentTopic("/reliable-channel/1/deferred-second/proto")

    var manager: ReliableChannelManager
    var subscribeRequests: seq[ContentTopic]
    var unsubscribeRequests: seq[ContentTopic]
    lockNewGlobalBrokerContext:
      let brokerCtx = globalBrokerContext()
      manager = ReliableChannelManager.new(ReliableChannelManagerConf()).expect(
          "ReliableChannelManager.new"
        )
      discard manager
        .createReliableChannel(firstChannelId, firstTopic, SdsParticipantID("local"))
        .expect("create first channel")
      discard manager
        .createReliableChannel(secondChannelId, secondTopic, SdsParticipantID("local"))
        .expect("create second channel")

      MessagingSubscribeChannel
        .setProvider(
          brokerCtx,
          proc(contentTopic: ContentTopic): Result[void, string] =
            subscribeRequests.add(contentTopic)
            if subscribeRequests.len == 2:
              return err("second deferred subscription failed")
            ok(),
        )
        .expect("setProvider MessagingSubscribeChannel")
      MessagingUnsubscribeChannel
        .setProvider(
          brokerCtx,
          proc(contentTopic: ContentTopic): Result[void, string] =
            unsubscribeRequests.add(contentTopic)
            ok(),
        )
        .expect("setProvider MessagingUnsubscribeChannel")

      let startResult = manager.start()
      check:
        startResult.isErr()
        startResult.error ==
          "failed to subscribe channel's content topic: second deferred subscription failed"
        subscribeRequests.len == 2
        unsubscribeRequests == @[subscribeRequests[0]]
        manager.deferredChannelSubscriptions.getOrDefault(firstChannelId)
        manager.deferredChannelSubscriptions.getOrDefault(secondChannelId)

      MessagingSubscribeChannel.clearProvider(brokerCtx)
      MessagingUnsubscribeChannel.clearProvider(brokerCtx)
      await manager.stop()

  asyncTest "stop releases active channel leases":
    const
      channelId = ChannelId("active-channel")
      topic = ContentTopic("/reliable-channel/1/active/proto")

    var manager: ReliableChannelManager
    var unsubscribed: seq[ContentTopic]
    lockNewGlobalBrokerContext:
      let brokerCtx = globalBrokerContext()
      manager = ReliableChannelManager.new(ReliableChannelManagerConf()).expect(
          "ReliableChannelManager.new"
        )
      MessagingSubscribeChannel
        .setProvider(
          brokerCtx,
          proc(_: ContentTopic): Result[void, string] =
            ok(),
        )
        .expect("setProvider MessagingSubscribeChannel")
      MessagingUnsubscribeChannel
        .setProvider(
          brokerCtx,
          proc(contentTopic: ContentTopic): Result[void, string] =
            unsubscribed.add(contentTopic)
            ok(),
        )
        .expect("setProvider MessagingUnsubscribeChannel")

      discard manager
        .createReliableChannel(channelId, topic, SdsParticipantID("local"))
        .expect("create channel")
      await manager.stop()
      check unsubscribed == @[topic]

      MessagingSubscribeChannel.clearProvider(brokerCtx)
      MessagingUnsubscribeChannel.clearProvider(brokerCtx)
