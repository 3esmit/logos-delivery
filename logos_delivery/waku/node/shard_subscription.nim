{.push raises: [].}

import std/[sets, tables]
import ../waku_core

type ShardSubscription* = object
  ## Aggregate content topics plus explicit owner leases. `contentTopics` is
  ## retained as the fast read-only view used by relay/filter reconciliation.
  contentTopics*: HashSet[ContentTopic]
  applicationContentTopics*: HashSet[ContentTopic]
  channelContentTopics*: Table[ContentTopic, int]
  sendContentTopics*: Table[ContentTopic, int]
  directShardSub*: bool
    ## shard subscribed directly (PubsubSub), independent of content-topic interest

{.pop.}
