{.used.}

import results, chronos, testutils/unittests
import brokers/broker_context

import ../testlib/[common, testasync]

import logos_delivery/api/conf/channels_conf
import logos_delivery/api/messaging_client_api
import logos_delivery/channels/reliable_channel_manager
import logos_delivery/channels/api/channel_lifecycle
import logos_delivery/channels/encryption/noop_encryption

suite "Reliable Channel - lifecycle":
  asyncTest "channelExists tracks create and close":
    const
      channelId = ChannelId("lifecycle-channel")
      contentTopic = ContentTopic("/reliable-channel/test/proto")

    var manager: ReliableChannelManager
    lockNewGlobalBrokerContext:
      manager = ReliableChannelManager.new(ReliableChannelManagerConf()).expect(
          "ReliableChannelManager.new"
        )
      setNoopEncryption()

      check not manager.channelExists(channelId)

      discard manager
        .createReliableChannel(channelId, contentTopic, SdsParticipantID("local"))
        .expect("createReliableChannel")

      check manager.channelExists(channelId)

      ## An unrelated id must not be reported as existing.
      check not manager.channelExists(ChannelId("other-channel"))

      (await manager.closeChannel(channelId)).expect("closeChannel")

      check not manager.channelExists(channelId)

  asyncTest "close preserves an application's content topic subscription":
    ## The subscription manager does not distinguish application-owned and
    ## channel-owned interest, so closing a channel must not remove an
    ## application's pre-existing subscription.
    const
      topic = ContentTopic("/reliable-channel/test/subscribe/proto")
      channelId = ChannelId("subscribe-channel")

    var subscribed: seq[ContentTopic]
    var unsubscribed: seq[ContentTopic]
    var manager: ReliableChannelManager
    lockNewGlobalBrokerContext:
      let brokerCtx = globalBrokerContext()
      manager = ReliableChannelManager.new(ReliableChannelManagerConf()).expect(
          "ReliableChannelManager.new"
        )
      setNoopEncryption()

      MessagingSubscribe
        .setProvider(
          brokerCtx,
          proc(contentTopic: ContentTopic): Result[void, string] =
            subscribed.add(contentTopic)
            ok(),
        )
        .expect("setProvider MessagingSubscribe")
      MessagingUnsubscribe
        .setProvider(
          brokerCtx,
          proc(contentTopic: ContentTopic): Result[void, string] =
            unsubscribed.add(contentTopic)
            ok(),
        )
        .expect("setProvider MessagingUnsubscribe")

      MessagingSubscribe.request(brokerCtx, topic).expect("application subscribe")

      discard manager
        .createReliableChannel(channelId, topic, SdsParticipantID("local"))
        .expect("createReliableChannel")

      check subscribed == @[topic, topic]

      (await manager.closeChannel(channelId)).expect("closeChannel")
      check unsubscribed.len == 0
