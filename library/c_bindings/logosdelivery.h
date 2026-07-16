#ifndef NIM_FFI_LIB_LOGOSDELIVERY_H_INCLUDED
#define NIM_FFI_LIB_LOGOSDELIVERY_H_INCLUDED
#include "nim_ffi_cbor.h"

/* ============================================================ */
/* Generated types (user-declared + per-proc request envelopes) */
/* ============================================================ */

typedef struct {
    NimFfiStr payload;
    NimFfiStr contentTopic;
    uint32_t version;
    int64_t timestamp;
    bool ephemeral;
    NimFfiStr meta;
    NimFfiStr proof;
} WakuMessagePayload;
typedef struct {
    NimFfiStr requestId;
    NimFfiStr messageHash;
} MessageSentPayload;
typedef struct {
    NimFfiStr requestId;
    NimFfiStr messageHash;
    NimFfiStr error;
} MessageErrorPayload;
typedef struct {
    NimFfiStr requestId;
    NimFfiStr messageHash;
} MessagePropagatedPayload;
typedef struct {
    NimFfiStr messageHash;
    WakuMessagePayload message;
} MessageReceivedPayload;
typedef struct {
    NimFfiStr connectionStatus;
} ConnectionStatusChangePayload;
typedef struct {
    NimFfiStr pubsubTopic;
    NimFfiStr topicHealth;
} TopicHealthChangePayload;
typedef struct {
    NimFfiStr peerId;
    NimFfiStr peerEvent;
} ConnectionChangePayload;
typedef struct {
    NimFfiStr channelId;
    NimFfiStr senderId;
    NimFfiStr payload;
} ChannelMessageReceivedPayload;
typedef struct {
    NimFfiStr channelId;
    NimFfiStr requestId;
} ChannelMessageSentPayload;
typedef struct {
    NimFfiStr channelId;
    NimFfiStr requestId;
    NimFfiStr error;
} ChannelMessageErrorPayload;
typedef struct {
    NimFfiStr pubsubTopic;
    NimFfiStr messageHash;
    WakuMessagePayload wakuMessage;
} ReceivedMessagePayload;
typedef struct {
    NimFfiStr contentTopic;
    NimFfiStr payload;
    bool ephemeral;
} SendRequest;
typedef struct {
    NimFfiStr payload;
    bool ephemeral;
} ChannelSendRequest;
typedef struct {
    NimFfiStr configJson;
} LogosdeliveryCreateNodeCtorReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} LogosdeliveryStartNodeReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} LogosdeliveryStopNodeReq;
typedef struct {
    NimFfiStr contentTopicStr;
} LogosdeliverySubscribeReq;
typedef struct {
    NimFfiStr contentTopicStr;
} LogosdeliveryUnsubscribeReq;
typedef struct {
    SendRequest req;
} LogosdeliverySendReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} LogosdeliveryGetAvailableNodeInfoIdsReq;
typedef struct {
    NimFfiStr nodeInfoId;
} LogosdeliveryGetNodeInfoReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} LogosdeliveryGetAvailableConfigsReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuGetPeeridsFromPeerstoreReq;
typedef struct {
    NimFfiStr peerMultiAddr;
    uint32_t timeoutMs;
} WakuConnectReq;
typedef struct {
    NimFfiStr peerId;
} WakuDisconnectPeerByIdReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuDisconnectAllPeersReq;
typedef struct {
    NimFfiStr peerMultiAddr;
    NimFfiStr protocol;
    uint32_t timeoutMs;
} WakuDialPeerReq;
typedef struct {
    NimFfiStr peerId;
    NimFfiStr protocol;
    uint32_t timeoutMs;
} WakuDialPeerByIdReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuGetConnectedPeersInfoReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuGetConnectedPeersReq;
typedef struct {
    NimFfiStr protocol;
} WakuGetPeeridsByProtocolReq;
typedef struct {
    NimFfiStr bootnodes;
} WakuDiscv5UpdateBootnodesReq;
typedef struct {
    NimFfiStr enrTreeUrl;
    NimFfiStr nameDnsServer;
    int32_t timeoutMs;
} WakuDnsDiscoveryReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuStartDiscv5Req;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuStopDiscv5Req;
typedef struct {
    uint64_t numPeers;
} WakuPeerExchangeRequestReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuVersionReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuListenAddressesReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuGetMyEnrReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuGetMyPeeridReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuGetMetricsReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuIsOnlineReq;
typedef struct {
    NimFfiStr peerAddr;
    uint32_t timeoutMs;
} WakuPingPeerReq;
typedef struct {
    NimFfiStr pubSubTopic;
} WakuRelayGetPeersInMeshReq;
typedef struct {
    NimFfiStr pubSubTopic;
} WakuRelayGetNumPeersInMeshReq;
typedef struct {
    NimFfiStr pubSubTopic;
} WakuRelayGetConnectedPeersReq;
typedef struct {
    NimFfiStr pubSubTopic;
} WakuRelayGetNumConnectedPeersReq;
typedef struct {
    int32_t clusterId;
    int32_t shardId;
    NimFfiStr publicKey;
} WakuRelayAddProtectedShardReq;
typedef struct {
    NimFfiStr pubSubTopic;
} WakuRelaySubscribeReq;
typedef struct {
    NimFfiStr pubSubTopic;
} WakuRelayUnsubscribeReq;
typedef struct {
    NimFfiStr pubSubTopic;
    NimFfiStr jsonWakuMessage;
    uint32_t timeoutMs;
} WakuRelayPublishReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuDefaultPubsubTopicReq;
typedef struct {
    NimFfiStr appName;
    uint32_t appVersion;
    NimFfiStr contentTopicName;
    NimFfiStr encoding;
} WakuContentTopicReq;
typedef struct {
    NimFfiStr topicName;
} WakuPubsubTopicReq;
typedef struct {
    NimFfiStr jsonQuery;
    NimFfiStr peerAddr;
    int32_t timeoutMs;
} WakuStoreQueryReq;
typedef struct {
    NimFfiStr pubSubTopic;
    NimFfiStr jsonWakuMessage;
} WakuLightpushPublishReq;
typedef struct {
    NimFfiStr pubSubTopic;
    NimFfiStr contentTopics;
} WakuFilterSubscribeReq;
typedef struct {
    NimFfiStr pubSubTopic;
    NimFfiStr contentTopics;
} WakuFilterUnsubscribeReq;
typedef struct {
    char _nimffi_empty; /* C forbids empty structs */
} WakuFilterUnsubscribeAllReq;
typedef struct {
    NimFfiStr channelIdStr;
    NimFfiStr contentTopicStr;
    NimFfiStr senderIdStr;
    NimFfiStr encryptionStr;
} LogosdeliveryChannelCreateReq;
typedef struct {
    NimFfiStr channelIdStr;
    ChannelSendRequest req;
} LogosdeliveryChannelSendReq;
typedef struct {
    NimFfiStr channelIdStr;
} LogosdeliveryChannelCloseReq;

static inline CborError logosdelivery_enc_WakuMessagePayload(
        CborEncoder* e, const WakuMessagePayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 7);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "payload");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->payload);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "version");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->version);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timestamp");
    if (err) return err;
    err = nimffi_enc_i64(&m, &v->timestamp);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "ephemeral");
    if (err) return err;
    err = nimffi_enc_bool(&m, &v->ephemeral);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "meta");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->meta);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "proof");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->proof);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuMessagePayload(
        CborValue* it, WakuMessagePayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "payload", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->payload);
    if (err) return err;
    err = cbor_value_map_find_value(it, "contentTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "version", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->version);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timestamp", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_i64(&field, &out->timestamp);
    if (err) return err;
    err = cbor_value_map_find_value(it, "ephemeral", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_bool(&field, &out->ephemeral);
    if (err) return err;
    err = cbor_value_map_find_value(it, "meta", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->meta);
    if (err) return err;
    err = cbor_value_map_find_value(it, "proof", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->proof);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuMessagePayload(WakuMessagePayload* v) {
    if (!v) return;
    nimffi_free_str(&v->payload);
    nimffi_free_str(&v->contentTopic);
    nimffi_free_str(&v->meta);
    nimffi_free_str(&v->proof);
}
static inline CborError logosdelivery_enc_MessageSentPayload(
        CborEncoder* e, const MessageSentPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "requestId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->requestId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "messageHash");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->messageHash);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_MessageSentPayload(
        CborValue* it, MessageSentPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "requestId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->requestId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "messageHash", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->messageHash);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_MessageSentPayload(MessageSentPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->requestId);
    nimffi_free_str(&v->messageHash);
}
static inline CborError logosdelivery_enc_MessageErrorPayload(
        CborEncoder* e, const MessageErrorPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "requestId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->requestId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "messageHash");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->messageHash);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "error");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->error);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_MessageErrorPayload(
        CborValue* it, MessageErrorPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "requestId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->requestId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "messageHash", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->messageHash);
    if (err) return err;
    err = cbor_value_map_find_value(it, "error", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->error);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_MessageErrorPayload(MessageErrorPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->requestId);
    nimffi_free_str(&v->messageHash);
    nimffi_free_str(&v->error);
}
static inline CborError logosdelivery_enc_MessagePropagatedPayload(
        CborEncoder* e, const MessagePropagatedPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "requestId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->requestId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "messageHash");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->messageHash);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_MessagePropagatedPayload(
        CborValue* it, MessagePropagatedPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "requestId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->requestId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "messageHash", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->messageHash);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_MessagePropagatedPayload(MessagePropagatedPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->requestId);
    nimffi_free_str(&v->messageHash);
}
static inline CborError logosdelivery_enc_MessageReceivedPayload(
        CborEncoder* e, const MessageReceivedPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "messageHash");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->messageHash);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "message");
    if (err) return err;
    err = logosdelivery_enc_WakuMessagePayload(&m, &v->message);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_MessageReceivedPayload(
        CborValue* it, MessageReceivedPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "messageHash", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->messageHash);
    if (err) return err;
    err = cbor_value_map_find_value(it, "message", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = logosdelivery_dec_WakuMessagePayload(&field, &out->message);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_MessageReceivedPayload(MessageReceivedPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->messageHash);
    logosdelivery_free_WakuMessagePayload(&v->message);
}
static inline CborError logosdelivery_enc_ConnectionStatusChangePayload(
        CborEncoder* e, const ConnectionStatusChangePayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "connectionStatus");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->connectionStatus);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ConnectionStatusChangePayload(
        CborValue* it, ConnectionStatusChangePayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "connectionStatus", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->connectionStatus);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ConnectionStatusChangePayload(ConnectionStatusChangePayload* v) {
    if (!v) return;
    nimffi_free_str(&v->connectionStatus);
}
static inline CborError logosdelivery_enc_TopicHealthChangePayload(
        CborEncoder* e, const TopicHealthChangePayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubsubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubsubTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "topicHealth");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->topicHealth);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_TopicHealthChangePayload(
        CborValue* it, TopicHealthChangePayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubsubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubsubTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "topicHealth", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->topicHealth);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_TopicHealthChangePayload(TopicHealthChangePayload* v) {
    if (!v) return;
    nimffi_free_str(&v->pubsubTopic);
    nimffi_free_str(&v->topicHealth);
}
static inline CborError logosdelivery_enc_ConnectionChangePayload(
        CborEncoder* e, const ConnectionChangePayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerEvent");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerEvent);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ConnectionChangePayload(
        CborValue* it, ConnectionChangePayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "peerId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "peerEvent", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerEvent);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ConnectionChangePayload(ConnectionChangePayload* v) {
    if (!v) return;
    nimffi_free_str(&v->peerId);
    nimffi_free_str(&v->peerEvent);
}
static inline CborError logosdelivery_enc_ChannelMessageReceivedPayload(
        CborEncoder* e, const ChannelMessageReceivedPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "channelId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->channelId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "senderId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->senderId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "payload");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->payload);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ChannelMessageReceivedPayload(
        CborValue* it, ChannelMessageReceivedPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "channelId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->channelId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "senderId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->senderId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "payload", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->payload);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ChannelMessageReceivedPayload(ChannelMessageReceivedPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->channelId);
    nimffi_free_str(&v->senderId);
    nimffi_free_str(&v->payload);
}
static inline CborError logosdelivery_enc_ChannelMessageSentPayload(
        CborEncoder* e, const ChannelMessageSentPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "channelId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->channelId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "requestId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->requestId);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ChannelMessageSentPayload(
        CborValue* it, ChannelMessageSentPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "channelId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->channelId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "requestId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->requestId);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ChannelMessageSentPayload(ChannelMessageSentPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->channelId);
    nimffi_free_str(&v->requestId);
}
static inline CborError logosdelivery_enc_ChannelMessageErrorPayload(
        CborEncoder* e, const ChannelMessageErrorPayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "channelId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->channelId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "requestId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->requestId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "error");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->error);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ChannelMessageErrorPayload(
        CborValue* it, ChannelMessageErrorPayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "channelId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->channelId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "requestId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->requestId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "error", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->error);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ChannelMessageErrorPayload(ChannelMessageErrorPayload* v) {
    if (!v) return;
    nimffi_free_str(&v->channelId);
    nimffi_free_str(&v->requestId);
    nimffi_free_str(&v->error);
}
static inline CborError logosdelivery_enc_ReceivedMessagePayload(
        CborEncoder* e, const ReceivedMessagePayload* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubsubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubsubTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "messageHash");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->messageHash);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "wakuMessage");
    if (err) return err;
    err = logosdelivery_enc_WakuMessagePayload(&m, &v->wakuMessage);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ReceivedMessagePayload(
        CborValue* it, ReceivedMessagePayload* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubsubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubsubTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "messageHash", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->messageHash);
    if (err) return err;
    err = cbor_value_map_find_value(it, "wakuMessage", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = logosdelivery_dec_WakuMessagePayload(&field, &out->wakuMessage);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ReceivedMessagePayload(ReceivedMessagePayload* v) {
    if (!v) return;
    nimffi_free_str(&v->pubsubTopic);
    nimffi_free_str(&v->messageHash);
    logosdelivery_free_WakuMessagePayload(&v->wakuMessage);
}
static inline CborError logosdelivery_enc_SendRequest(
        CborEncoder* e, const SendRequest* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "payload");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->payload);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "ephemeral");
    if (err) return err;
    err = nimffi_enc_bool(&m, &v->ephemeral);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_SendRequest(
        CborValue* it, SendRequest* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "contentTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "payload", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->payload);
    if (err) return err;
    err = cbor_value_map_find_value(it, "ephemeral", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_bool(&field, &out->ephemeral);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_SendRequest(SendRequest* v) {
    if (!v) return;
    nimffi_free_str(&v->contentTopic);
    nimffi_free_str(&v->payload);
}
static inline CborError logosdelivery_enc_ChannelSendRequest(
        CborEncoder* e, const ChannelSendRequest* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "payload");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->payload);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "ephemeral");
    if (err) return err;
    err = nimffi_enc_bool(&m, &v->ephemeral);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_ChannelSendRequest(
        CborValue* it, ChannelSendRequest* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "payload", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->payload);
    if (err) return err;
    err = cbor_value_map_find_value(it, "ephemeral", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_bool(&field, &out->ephemeral);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_ChannelSendRequest(ChannelSendRequest* v) {
    if (!v) return;
    nimffi_free_str(&v->payload);
}
static inline CborError logosdelivery_enc_LogosdeliveryCreateNodeCtorReq(
        CborEncoder* e, const LogosdeliveryCreateNodeCtorReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "configJson");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->configJson);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryCreateNodeCtorReq(
        CborValue* it, LogosdeliveryCreateNodeCtorReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "configJson", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->configJson);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliveryCreateNodeCtorReq(LogosdeliveryCreateNodeCtorReq* v) {
    if (!v) return;
    nimffi_free_str(&v->configJson);
}
static inline CborError logosdelivery_enc_LogosdeliveryStartNodeReq(
        CborEncoder* e, const LogosdeliveryStartNodeReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryStartNodeReq(
        CborValue* it, LogosdeliveryStartNodeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_LogosdeliveryStopNodeReq(
        CborEncoder* e, const LogosdeliveryStopNodeReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryStopNodeReq(
        CborValue* it, LogosdeliveryStopNodeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_LogosdeliverySubscribeReq(
        CborEncoder* e, const LogosdeliverySubscribeReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopicStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopicStr);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliverySubscribeReq(
        CborValue* it, LogosdeliverySubscribeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "contentTopicStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopicStr);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliverySubscribeReq(LogosdeliverySubscribeReq* v) {
    if (!v) return;
    nimffi_free_str(&v->contentTopicStr);
}
static inline CborError logosdelivery_enc_LogosdeliveryUnsubscribeReq(
        CborEncoder* e, const LogosdeliveryUnsubscribeReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopicStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopicStr);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryUnsubscribeReq(
        CborValue* it, LogosdeliveryUnsubscribeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "contentTopicStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopicStr);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliveryUnsubscribeReq(LogosdeliveryUnsubscribeReq* v) {
    if (!v) return;
    nimffi_free_str(&v->contentTopicStr);
}
static inline CborError logosdelivery_enc_LogosdeliverySendReq(
        CborEncoder* e, const LogosdeliverySendReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "req");
    if (err) return err;
    err = logosdelivery_enc_SendRequest(&m, &v->req);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliverySendReq(
        CborValue* it, LogosdeliverySendReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "req", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = logosdelivery_dec_SendRequest(&field, &out->req);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliverySendReq(LogosdeliverySendReq* v) {
    if (!v) return;
    logosdelivery_free_SendRequest(&v->req);
}
static inline CborError logosdelivery_enc_LogosdeliveryGetAvailableNodeInfoIdsReq(
        CborEncoder* e, const LogosdeliveryGetAvailableNodeInfoIdsReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryGetAvailableNodeInfoIdsReq(
        CborValue* it, LogosdeliveryGetAvailableNodeInfoIdsReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_LogosdeliveryGetNodeInfoReq(
        CborEncoder* e, const LogosdeliveryGetNodeInfoReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "nodeInfoId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->nodeInfoId);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryGetNodeInfoReq(
        CborValue* it, LogosdeliveryGetNodeInfoReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "nodeInfoId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->nodeInfoId);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliveryGetNodeInfoReq(LogosdeliveryGetNodeInfoReq* v) {
    if (!v) return;
    nimffi_free_str(&v->nodeInfoId);
}
static inline CborError logosdelivery_enc_LogosdeliveryGetAvailableConfigsReq(
        CborEncoder* e, const LogosdeliveryGetAvailableConfigsReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryGetAvailableConfigsReq(
        CborValue* it, LogosdeliveryGetAvailableConfigsReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuGetPeeridsFromPeerstoreReq(
        CborEncoder* e, const WakuGetPeeridsFromPeerstoreReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetPeeridsFromPeerstoreReq(
        CborValue* it, WakuGetPeeridsFromPeerstoreReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuConnectReq(
        CborEncoder* e, const WakuConnectReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerMultiAddr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerMultiAddr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuConnectReq(
        CborValue* it, WakuConnectReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "peerMultiAddr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerMultiAddr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuConnectReq(WakuConnectReq* v) {
    if (!v) return;
    nimffi_free_str(&v->peerMultiAddr);
}
static inline CborError logosdelivery_enc_WakuDisconnectPeerByIdReq(
        CborEncoder* e, const WakuDisconnectPeerByIdReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerId);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDisconnectPeerByIdReq(
        CborValue* it, WakuDisconnectPeerByIdReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "peerId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerId);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuDisconnectPeerByIdReq(WakuDisconnectPeerByIdReq* v) {
    if (!v) return;
    nimffi_free_str(&v->peerId);
}
static inline CborError logosdelivery_enc_WakuDisconnectAllPeersReq(
        CborEncoder* e, const WakuDisconnectAllPeersReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDisconnectAllPeersReq(
        CborValue* it, WakuDisconnectAllPeersReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuDialPeerReq(
        CborEncoder* e, const WakuDialPeerReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerMultiAddr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerMultiAddr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "protocol");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->protocol);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDialPeerReq(
        CborValue* it, WakuDialPeerReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "peerMultiAddr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerMultiAddr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "protocol", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->protocol);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuDialPeerReq(WakuDialPeerReq* v) {
    if (!v) return;
    nimffi_free_str(&v->peerMultiAddr);
    nimffi_free_str(&v->protocol);
}
static inline CborError logosdelivery_enc_WakuDialPeerByIdReq(
        CborEncoder* e, const WakuDialPeerByIdReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerId");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "protocol");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->protocol);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDialPeerByIdReq(
        CborValue* it, WakuDialPeerByIdReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "peerId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "protocol", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->protocol);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuDialPeerByIdReq(WakuDialPeerByIdReq* v) {
    if (!v) return;
    nimffi_free_str(&v->peerId);
    nimffi_free_str(&v->protocol);
}
static inline CborError logosdelivery_enc_WakuGetConnectedPeersInfoReq(
        CborEncoder* e, const WakuGetConnectedPeersInfoReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetConnectedPeersInfoReq(
        CborValue* it, WakuGetConnectedPeersInfoReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuGetConnectedPeersReq(
        CborEncoder* e, const WakuGetConnectedPeersReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetConnectedPeersReq(
        CborValue* it, WakuGetConnectedPeersReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuGetPeeridsByProtocolReq(
        CborEncoder* e, const WakuGetPeeridsByProtocolReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "protocol");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->protocol);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetPeeridsByProtocolReq(
        CborValue* it, WakuGetPeeridsByProtocolReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "protocol", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->protocol);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuGetPeeridsByProtocolReq(WakuGetPeeridsByProtocolReq* v) {
    if (!v) return;
    nimffi_free_str(&v->protocol);
}
static inline CborError logosdelivery_enc_WakuDiscv5UpdateBootnodesReq(
        CborEncoder* e, const WakuDiscv5UpdateBootnodesReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "bootnodes");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->bootnodes);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDiscv5UpdateBootnodesReq(
        CborValue* it, WakuDiscv5UpdateBootnodesReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "bootnodes", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->bootnodes);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuDiscv5UpdateBootnodesReq(WakuDiscv5UpdateBootnodesReq* v) {
    if (!v) return;
    nimffi_free_str(&v->bootnodes);
}
static inline CborError logosdelivery_enc_WakuDnsDiscoveryReq(
        CborEncoder* e, const WakuDnsDiscoveryReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "enrTreeUrl");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->enrTreeUrl);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "nameDnsServer");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->nameDnsServer);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_i32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDnsDiscoveryReq(
        CborValue* it, WakuDnsDiscoveryReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "enrTreeUrl", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->enrTreeUrl);
    if (err) return err;
    err = cbor_value_map_find_value(it, "nameDnsServer", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->nameDnsServer);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_i32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuDnsDiscoveryReq(WakuDnsDiscoveryReq* v) {
    if (!v) return;
    nimffi_free_str(&v->enrTreeUrl);
    nimffi_free_str(&v->nameDnsServer);
}
static inline CborError logosdelivery_enc_WakuStartDiscv5Req(
        CborEncoder* e, const WakuStartDiscv5Req* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuStartDiscv5Req(
        CborValue* it, WakuStartDiscv5Req* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuStopDiscv5Req(
        CborEncoder* e, const WakuStopDiscv5Req* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuStopDiscv5Req(
        CborValue* it, WakuStopDiscv5Req* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuPeerExchangeRequestReq(
        CborEncoder* e, const WakuPeerExchangeRequestReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "numPeers");
    if (err) return err;
    err = nimffi_enc_u64(&m, &v->numPeers);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuPeerExchangeRequestReq(
        CborValue* it, WakuPeerExchangeRequestReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "numPeers", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u64(&field, &out->numPeers);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuVersionReq(
        CborEncoder* e, const WakuVersionReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuVersionReq(
        CborValue* it, WakuVersionReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuListenAddressesReq(
        CborEncoder* e, const WakuListenAddressesReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuListenAddressesReq(
        CborValue* it, WakuListenAddressesReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuGetMyEnrReq(
        CborEncoder* e, const WakuGetMyEnrReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetMyEnrReq(
        CborValue* it, WakuGetMyEnrReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuGetMyPeeridReq(
        CborEncoder* e, const WakuGetMyPeeridReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetMyPeeridReq(
        CborValue* it, WakuGetMyPeeridReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuGetMetricsReq(
        CborEncoder* e, const WakuGetMetricsReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuGetMetricsReq(
        CborValue* it, WakuGetMetricsReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuIsOnlineReq(
        CborEncoder* e, const WakuIsOnlineReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuIsOnlineReq(
        CborValue* it, WakuIsOnlineReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuPingPeerReq(
        CborEncoder* e, const WakuPingPeerReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerAddr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerAddr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuPingPeerReq(
        CborValue* it, WakuPingPeerReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "peerAddr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerAddr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuPingPeerReq(WakuPingPeerReq* v) {
    if (!v) return;
    nimffi_free_str(&v->peerAddr);
}
static inline CborError logosdelivery_enc_WakuRelayGetPeersInMeshReq(
        CborEncoder* e, const WakuRelayGetPeersInMeshReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayGetPeersInMeshReq(
        CborValue* it, WakuRelayGetPeersInMeshReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayGetPeersInMeshReq(WakuRelayGetPeersInMeshReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
}
static inline CborError logosdelivery_enc_WakuRelayGetNumPeersInMeshReq(
        CborEncoder* e, const WakuRelayGetNumPeersInMeshReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayGetNumPeersInMeshReq(
        CborValue* it, WakuRelayGetNumPeersInMeshReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayGetNumPeersInMeshReq(WakuRelayGetNumPeersInMeshReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
}
static inline CborError logosdelivery_enc_WakuRelayGetConnectedPeersReq(
        CborEncoder* e, const WakuRelayGetConnectedPeersReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayGetConnectedPeersReq(
        CborValue* it, WakuRelayGetConnectedPeersReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayGetConnectedPeersReq(WakuRelayGetConnectedPeersReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
}
static inline CborError logosdelivery_enc_WakuRelayGetNumConnectedPeersReq(
        CborEncoder* e, const WakuRelayGetNumConnectedPeersReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayGetNumConnectedPeersReq(
        CborValue* it, WakuRelayGetNumConnectedPeersReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayGetNumConnectedPeersReq(WakuRelayGetNumConnectedPeersReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
}
static inline CborError logosdelivery_enc_WakuRelayAddProtectedShardReq(
        CborEncoder* e, const WakuRelayAddProtectedShardReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "clusterId");
    if (err) return err;
    err = nimffi_enc_i32(&m, &v->clusterId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "shardId");
    if (err) return err;
    err = nimffi_enc_i32(&m, &v->shardId);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "publicKey");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->publicKey);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayAddProtectedShardReq(
        CborValue* it, WakuRelayAddProtectedShardReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "clusterId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_i32(&field, &out->clusterId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "shardId", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_i32(&field, &out->shardId);
    if (err) return err;
    err = cbor_value_map_find_value(it, "publicKey", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->publicKey);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayAddProtectedShardReq(WakuRelayAddProtectedShardReq* v) {
    if (!v) return;
    nimffi_free_str(&v->publicKey);
}
static inline CborError logosdelivery_enc_WakuRelaySubscribeReq(
        CborEncoder* e, const WakuRelaySubscribeReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelaySubscribeReq(
        CborValue* it, WakuRelaySubscribeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelaySubscribeReq(WakuRelaySubscribeReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
}
static inline CborError logosdelivery_enc_WakuRelayUnsubscribeReq(
        CborEncoder* e, const WakuRelayUnsubscribeReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayUnsubscribeReq(
        CborValue* it, WakuRelayUnsubscribeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayUnsubscribeReq(WakuRelayUnsubscribeReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
}
static inline CborError logosdelivery_enc_WakuRelayPublishReq(
        CborEncoder* e, const WakuRelayPublishReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "jsonWakuMessage");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->jsonWakuMessage);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuRelayPublishReq(
        CborValue* it, WakuRelayPublishReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "jsonWakuMessage", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->jsonWakuMessage);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuRelayPublishReq(WakuRelayPublishReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
    nimffi_free_str(&v->jsonWakuMessage);
}
static inline CborError logosdelivery_enc_WakuDefaultPubsubTopicReq(
        CborEncoder* e, const WakuDefaultPubsubTopicReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuDefaultPubsubTopicReq(
        CborValue* it, WakuDefaultPubsubTopicReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_WakuContentTopicReq(
        CborEncoder* e, const WakuContentTopicReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 4);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "appName");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->appName);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "appVersion");
    if (err) return err;
    err = nimffi_enc_u32(&m, &v->appVersion);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopicName");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopicName);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "encoding");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->encoding);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuContentTopicReq(
        CborValue* it, WakuContentTopicReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "appName", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->appName);
    if (err) return err;
    err = cbor_value_map_find_value(it, "appVersion", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_u32(&field, &out->appVersion);
    if (err) return err;
    err = cbor_value_map_find_value(it, "contentTopicName", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopicName);
    if (err) return err;
    err = cbor_value_map_find_value(it, "encoding", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->encoding);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuContentTopicReq(WakuContentTopicReq* v) {
    if (!v) return;
    nimffi_free_str(&v->appName);
    nimffi_free_str(&v->contentTopicName);
    nimffi_free_str(&v->encoding);
}
static inline CborError logosdelivery_enc_WakuPubsubTopicReq(
        CborEncoder* e, const WakuPubsubTopicReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "topicName");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->topicName);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuPubsubTopicReq(
        CborValue* it, WakuPubsubTopicReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "topicName", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->topicName);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuPubsubTopicReq(WakuPubsubTopicReq* v) {
    if (!v) return;
    nimffi_free_str(&v->topicName);
}
static inline CborError logosdelivery_enc_WakuStoreQueryReq(
        CborEncoder* e, const WakuStoreQueryReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 3);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "jsonQuery");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->jsonQuery);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "peerAddr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->peerAddr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "timeoutMs");
    if (err) return err;
    err = nimffi_enc_i32(&m, &v->timeoutMs);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuStoreQueryReq(
        CborValue* it, WakuStoreQueryReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "jsonQuery", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->jsonQuery);
    if (err) return err;
    err = cbor_value_map_find_value(it, "peerAddr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->peerAddr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "timeoutMs", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_i32(&field, &out->timeoutMs);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuStoreQueryReq(WakuStoreQueryReq* v) {
    if (!v) return;
    nimffi_free_str(&v->jsonQuery);
    nimffi_free_str(&v->peerAddr);
}
static inline CborError logosdelivery_enc_WakuLightpushPublishReq(
        CborEncoder* e, const WakuLightpushPublishReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "jsonWakuMessage");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->jsonWakuMessage);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuLightpushPublishReq(
        CborValue* it, WakuLightpushPublishReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "jsonWakuMessage", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->jsonWakuMessage);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuLightpushPublishReq(WakuLightpushPublishReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
    nimffi_free_str(&v->jsonWakuMessage);
}
static inline CborError logosdelivery_enc_WakuFilterSubscribeReq(
        CborEncoder* e, const WakuFilterSubscribeReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopics");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopics);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuFilterSubscribeReq(
        CborValue* it, WakuFilterSubscribeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "contentTopics", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopics);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuFilterSubscribeReq(WakuFilterSubscribeReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
    nimffi_free_str(&v->contentTopics);
}
static inline CborError logosdelivery_enc_WakuFilterUnsubscribeReq(
        CborEncoder* e, const WakuFilterUnsubscribeReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "pubSubTopic");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->pubSubTopic);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopics");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopics);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuFilterUnsubscribeReq(
        CborValue* it, WakuFilterUnsubscribeReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "pubSubTopic", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->pubSubTopic);
    if (err) return err;
    err = cbor_value_map_find_value(it, "contentTopics", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopics);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_WakuFilterUnsubscribeReq(WakuFilterUnsubscribeReq* v) {
    if (!v) return;
    nimffi_free_str(&v->pubSubTopic);
    nimffi_free_str(&v->contentTopics);
}
static inline CborError logosdelivery_enc_WakuFilterUnsubscribeAllReq(
        CborEncoder* e, const WakuFilterUnsubscribeAllReq* v) {
    (void)v;
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 0);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_WakuFilterUnsubscribeAllReq(
        CborValue* it, WakuFilterUnsubscribeAllReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    (void)out;
    return cbor_value_advance(it);
}
static inline CborError logosdelivery_enc_LogosdeliveryChannelCreateReq(
        CborEncoder* e, const LogosdeliveryChannelCreateReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 4);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "channelIdStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->channelIdStr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "contentTopicStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->contentTopicStr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "senderIdStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->senderIdStr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "encryptionStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->encryptionStr);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryChannelCreateReq(
        CborValue* it, LogosdeliveryChannelCreateReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "channelIdStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->channelIdStr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "contentTopicStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->contentTopicStr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "senderIdStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->senderIdStr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "encryptionStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->encryptionStr);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliveryChannelCreateReq(LogosdeliveryChannelCreateReq* v) {
    if (!v) return;
    nimffi_free_str(&v->channelIdStr);
    nimffi_free_str(&v->contentTopicStr);
    nimffi_free_str(&v->senderIdStr);
    nimffi_free_str(&v->encryptionStr);
}
static inline CborError logosdelivery_enc_LogosdeliveryChannelSendReq(
        CborEncoder* e, const LogosdeliveryChannelSendReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 2);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "channelIdStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->channelIdStr);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "req");
    if (err) return err;
    err = logosdelivery_enc_ChannelSendRequest(&m, &v->req);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryChannelSendReq(
        CborValue* it, LogosdeliveryChannelSendReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "channelIdStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->channelIdStr);
    if (err) return err;
    err = cbor_value_map_find_value(it, "req", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = logosdelivery_dec_ChannelSendRequest(&field, &out->req);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliveryChannelSendReq(LogosdeliveryChannelSendReq* v) {
    if (!v) return;
    nimffi_free_str(&v->channelIdStr);
    logosdelivery_free_ChannelSendRequest(&v->req);
}
static inline CborError logosdelivery_enc_LogosdeliveryChannelCloseReq(
        CborEncoder* e, const LogosdeliveryChannelCloseReq* v) {
    CborEncoder m;
    CborError err = cbor_encoder_create_map(e, &m, 1);
    if (err) return err;
    err = cbor_encode_text_stringz(&m, "channelIdStr");
    if (err) return err;
    err = nimffi_enc_str(&m, &v->channelIdStr);
    if (err) return err;
    return cbor_encoder_close_container(e, &m);
}
static inline CborError logosdelivery_dec_LogosdeliveryChannelCloseReq(
        CborValue* it, LogosdeliveryChannelCloseReq* out) {
    if (!cbor_value_is_map(it)) return CborErrorImproperValue;
    CborValue field;
    CborError err;
    err = cbor_value_map_find_value(it, "channelIdStr", &field);
    if (err) return err;
    if (!cbor_value_is_valid(&field)) return CborErrorImproperValue;
    err = nimffi_dec_str(&field, &out->channelIdStr);
    if (err) return err;
    return cbor_value_advance(it);
}
static inline void logosdelivery_free_LogosdeliveryChannelCloseReq(LogosdeliveryChannelCloseReq* v) {
    if (!v) return;
    nimffi_free_str(&v->channelIdStr);
}

/* ============================================================ */
/* C ABI declarations (symbols exported by the Nim dylib)       */
/* ============================================================ */
#ifdef __cplusplus
extern "C" {
#endif

void* logosdelivery_create_node(const uint8_t* req_cbor, size_t req_cbor_len, FFICallback callback, void* user_data);
int logosdelivery_start_node(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_stop_node(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_destroy(void* ctx);
int logosdelivery_subscribe(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_unsubscribe(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_send(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_get_available_node_info_ids(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_get_node_info(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_get_available_configs(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_peerids_from_peerstore(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_connect(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_disconnect_peer_by_id(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_disconnect_all_peers(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_dial_peer(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_dial_peer_by_id(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_connected_peers_info(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_connected_peers(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_peerids_by_protocol(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_discv5_update_bootnodes(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_dns_discovery(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_start_discv5(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_stop_discv5(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_peer_exchange_request(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_version(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_listen_addresses(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_my_enr(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_my_peerid(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_get_metrics(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_is_online(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_ping_peer(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_get_peers_in_mesh(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_get_num_peers_in_mesh(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_get_connected_peers(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_get_num_connected_peers(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_add_protected_shard(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_subscribe(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_unsubscribe(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_relay_publish(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_default_pubsub_topic(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_content_topic(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_pubsub_topic(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_store_query(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_lightpush_publish(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_filter_subscribe(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_filter_unsubscribe(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int waku_filter_unsubscribe_all(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_channel_create(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_channel_send(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
int logosdelivery_channel_close(void* ctx, FFICallback callback, void* user_data, const uint8_t* req_cbor, size_t req_cbor_len);
uint64_t logosdelivery_add_event_listener(void* ctx, const char* event_name, FFICallback callback, void* user_data);
int logosdelivery_remove_event_listener(void* ctx, uint64_t listener_id);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* CBOR buffer adapters (typed codec → void* driver signature) */
static inline CborError logosdelivery_encv_LogosdeliveryCreateNodeCtorReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryCreateNodeCtorReq(e, (const LogosdeliveryCreateNodeCtorReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryStartNodeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryStartNodeReq(e, (const LogosdeliveryStartNodeReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryStopNodeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryStopNodeReq(e, (const LogosdeliveryStopNodeReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliverySubscribeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliverySubscribeReq(e, (const LogosdeliverySubscribeReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryUnsubscribeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryUnsubscribeReq(e, (const LogosdeliveryUnsubscribeReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliverySendReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliverySendReq(e, (const LogosdeliverySendReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryGetAvailableNodeInfoIdsReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryGetAvailableNodeInfoIdsReq(e, (const LogosdeliveryGetAvailableNodeInfoIdsReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryGetNodeInfoReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryGetNodeInfoReq(e, (const LogosdeliveryGetNodeInfoReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryGetAvailableConfigsReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryGetAvailableConfigsReq(e, (const LogosdeliveryGetAvailableConfigsReq*)v); }
static inline CborError logosdelivery_encv_WakuGetPeeridsFromPeerstoreReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetPeeridsFromPeerstoreReq(e, (const WakuGetPeeridsFromPeerstoreReq*)v); }
static inline CborError logosdelivery_encv_WakuConnectReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuConnectReq(e, (const WakuConnectReq*)v); }
static inline CborError logosdelivery_encv_WakuDisconnectPeerByIdReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDisconnectPeerByIdReq(e, (const WakuDisconnectPeerByIdReq*)v); }
static inline CborError logosdelivery_encv_WakuDisconnectAllPeersReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDisconnectAllPeersReq(e, (const WakuDisconnectAllPeersReq*)v); }
static inline CborError logosdelivery_encv_WakuDialPeerReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDialPeerReq(e, (const WakuDialPeerReq*)v); }
static inline CborError logosdelivery_encv_WakuDialPeerByIdReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDialPeerByIdReq(e, (const WakuDialPeerByIdReq*)v); }
static inline CborError logosdelivery_encv_WakuGetConnectedPeersInfoReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetConnectedPeersInfoReq(e, (const WakuGetConnectedPeersInfoReq*)v); }
static inline CborError logosdelivery_encv_WakuGetConnectedPeersReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetConnectedPeersReq(e, (const WakuGetConnectedPeersReq*)v); }
static inline CborError logosdelivery_encv_WakuGetPeeridsByProtocolReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetPeeridsByProtocolReq(e, (const WakuGetPeeridsByProtocolReq*)v); }
static inline CborError logosdelivery_encv_WakuDiscv5UpdateBootnodesReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDiscv5UpdateBootnodesReq(e, (const WakuDiscv5UpdateBootnodesReq*)v); }
static inline CborError logosdelivery_encv_WakuDnsDiscoveryReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDnsDiscoveryReq(e, (const WakuDnsDiscoveryReq*)v); }
static inline CborError logosdelivery_encv_WakuStartDiscv5Req(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuStartDiscv5Req(e, (const WakuStartDiscv5Req*)v); }
static inline CborError logosdelivery_encv_WakuStopDiscv5Req(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuStopDiscv5Req(e, (const WakuStopDiscv5Req*)v); }
static inline CborError logosdelivery_encv_WakuPeerExchangeRequestReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuPeerExchangeRequestReq(e, (const WakuPeerExchangeRequestReq*)v); }
static inline CborError logosdelivery_encv_WakuVersionReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuVersionReq(e, (const WakuVersionReq*)v); }
static inline CborError logosdelivery_encv_WakuListenAddressesReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuListenAddressesReq(e, (const WakuListenAddressesReq*)v); }
static inline CborError logosdelivery_encv_WakuGetMyEnrReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetMyEnrReq(e, (const WakuGetMyEnrReq*)v); }
static inline CborError logosdelivery_encv_WakuGetMyPeeridReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetMyPeeridReq(e, (const WakuGetMyPeeridReq*)v); }
static inline CborError logosdelivery_encv_WakuGetMetricsReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuGetMetricsReq(e, (const WakuGetMetricsReq*)v); }
static inline CborError logosdelivery_encv_WakuIsOnlineReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuIsOnlineReq(e, (const WakuIsOnlineReq*)v); }
static inline CborError logosdelivery_encv_WakuPingPeerReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuPingPeerReq(e, (const WakuPingPeerReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayGetPeersInMeshReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayGetPeersInMeshReq(e, (const WakuRelayGetPeersInMeshReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayGetNumPeersInMeshReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayGetNumPeersInMeshReq(e, (const WakuRelayGetNumPeersInMeshReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayGetConnectedPeersReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayGetConnectedPeersReq(e, (const WakuRelayGetConnectedPeersReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayGetNumConnectedPeersReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayGetNumConnectedPeersReq(e, (const WakuRelayGetNumConnectedPeersReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayAddProtectedShardReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayAddProtectedShardReq(e, (const WakuRelayAddProtectedShardReq*)v); }
static inline CborError logosdelivery_encv_WakuRelaySubscribeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelaySubscribeReq(e, (const WakuRelaySubscribeReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayUnsubscribeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayUnsubscribeReq(e, (const WakuRelayUnsubscribeReq*)v); }
static inline CborError logosdelivery_encv_WakuRelayPublishReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuRelayPublishReq(e, (const WakuRelayPublishReq*)v); }
static inline CborError logosdelivery_encv_WakuDefaultPubsubTopicReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuDefaultPubsubTopicReq(e, (const WakuDefaultPubsubTopicReq*)v); }
static inline CborError logosdelivery_encv_WakuContentTopicReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuContentTopicReq(e, (const WakuContentTopicReq*)v); }
static inline CborError logosdelivery_encv_WakuPubsubTopicReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuPubsubTopicReq(e, (const WakuPubsubTopicReq*)v); }
static inline CborError logosdelivery_encv_WakuStoreQueryReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuStoreQueryReq(e, (const WakuStoreQueryReq*)v); }
static inline CborError logosdelivery_encv_WakuLightpushPublishReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuLightpushPublishReq(e, (const WakuLightpushPublishReq*)v); }
static inline CborError logosdelivery_encv_WakuFilterSubscribeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuFilterSubscribeReq(e, (const WakuFilterSubscribeReq*)v); }
static inline CborError logosdelivery_encv_WakuFilterUnsubscribeReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuFilterUnsubscribeReq(e, (const WakuFilterUnsubscribeReq*)v); }
static inline CborError logosdelivery_encv_WakuFilterUnsubscribeAllReq(CborEncoder* e, const void* v) { return logosdelivery_enc_WakuFilterUnsubscribeAllReq(e, (const WakuFilterUnsubscribeAllReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryChannelCreateReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryChannelCreateReq(e, (const LogosdeliveryChannelCreateReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryChannelSendReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryChannelSendReq(e, (const LogosdeliveryChannelSendReq*)v); }
static inline CborError logosdelivery_encv_LogosdeliveryChannelCloseReq(CborEncoder* e, const void* v) { return logosdelivery_enc_LogosdeliveryChannelCloseReq(e, (const LogosdeliveryChannelCloseReq*)v); }
static inline CborError logosdelivery_decv_Str(CborValue* it, void* v) { return nimffi_dec_str(it, (NimFfiStr*)v); }

/* Event listener machinery */
typedef void (*LogosDeliveryOnMessageSentFn)(const MessageSentPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnMessageSentFn fn; void* user_data; } LogosDeliveryOnMessageSentBox;
static void logosdelivery_on_message_sent_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnMessageSentBox* box = (LogosDeliveryOnMessageSentBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    MessageSentPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_MessageSentPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_MessageSentPayload(&payload);
}

typedef void (*LogosDeliveryOnMessageErrorFn)(const MessageErrorPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnMessageErrorFn fn; void* user_data; } LogosDeliveryOnMessageErrorBox;
static void logosdelivery_on_message_error_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnMessageErrorBox* box = (LogosDeliveryOnMessageErrorBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    MessageErrorPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_MessageErrorPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_MessageErrorPayload(&payload);
}

typedef void (*LogosDeliveryOnMessagePropagatedFn)(const MessagePropagatedPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnMessagePropagatedFn fn; void* user_data; } LogosDeliveryOnMessagePropagatedBox;
static void logosdelivery_on_message_propagated_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnMessagePropagatedBox* box = (LogosDeliveryOnMessagePropagatedBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    MessagePropagatedPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_MessagePropagatedPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_MessagePropagatedPayload(&payload);
}

typedef void (*LogosDeliveryOnMessageReceivedFn)(const MessageReceivedPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnMessageReceivedFn fn; void* user_data; } LogosDeliveryOnMessageReceivedBox;
static void logosdelivery_on_message_received_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnMessageReceivedBox* box = (LogosDeliveryOnMessageReceivedBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    MessageReceivedPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_MessageReceivedPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_MessageReceivedPayload(&payload);
}

typedef void (*LogosDeliveryOnConnectionStatusChangeFn)(const ConnectionStatusChangePayload* evt, void* user_data);
typedef struct { LogosDeliveryOnConnectionStatusChangeFn fn; void* user_data; } LogosDeliveryOnConnectionStatusChangeBox;
static void logosdelivery_on_connection_status_change_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnConnectionStatusChangeBox* box = (LogosDeliveryOnConnectionStatusChangeBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    ConnectionStatusChangePayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_ConnectionStatusChangePayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_ConnectionStatusChangePayload(&payload);
}

typedef void (*LogosDeliveryOnTopicHealthChangeFn)(const TopicHealthChangePayload* evt, void* user_data);
typedef struct { LogosDeliveryOnTopicHealthChangeFn fn; void* user_data; } LogosDeliveryOnTopicHealthChangeBox;
static void logosdelivery_on_topic_health_change_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnTopicHealthChangeBox* box = (LogosDeliveryOnTopicHealthChangeBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    TopicHealthChangePayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_TopicHealthChangePayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_TopicHealthChangePayload(&payload);
}

typedef void (*LogosDeliveryOnConnectionChangeFn)(const ConnectionChangePayload* evt, void* user_data);
typedef struct { LogosDeliveryOnConnectionChangeFn fn; void* user_data; } LogosDeliveryOnConnectionChangeBox;
static void logosdelivery_on_connection_change_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnConnectionChangeBox* box = (LogosDeliveryOnConnectionChangeBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    ConnectionChangePayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_ConnectionChangePayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_ConnectionChangePayload(&payload);
}

typedef void (*LogosDeliveryOnChannelMessageReceivedFn)(const ChannelMessageReceivedPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnChannelMessageReceivedFn fn; void* user_data; } LogosDeliveryOnChannelMessageReceivedBox;
static void logosdelivery_on_channel_message_received_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnChannelMessageReceivedBox* box = (LogosDeliveryOnChannelMessageReceivedBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    ChannelMessageReceivedPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_ChannelMessageReceivedPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_ChannelMessageReceivedPayload(&payload);
}

typedef void (*LogosDeliveryOnChannelMessageSentFn)(const ChannelMessageSentPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnChannelMessageSentFn fn; void* user_data; } LogosDeliveryOnChannelMessageSentBox;
static void logosdelivery_on_channel_message_sent_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnChannelMessageSentBox* box = (LogosDeliveryOnChannelMessageSentBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    ChannelMessageSentPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_ChannelMessageSentPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_ChannelMessageSentPayload(&payload);
}

typedef void (*LogosDeliveryOnChannelMessageErrorFn)(const ChannelMessageErrorPayload* evt, void* user_data);
typedef struct { LogosDeliveryOnChannelMessageErrorFn fn; void* user_data; } LogosDeliveryOnChannelMessageErrorBox;
static void logosdelivery_on_channel_message_error_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnChannelMessageErrorBox* box = (LogosDeliveryOnChannelMessageErrorBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    ChannelMessageErrorPayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_ChannelMessageErrorPayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_ChannelMessageErrorPayload(&payload);
}

typedef void (*LogosDeliveryOnReceivedMessageFn)(const ReceivedMessagePayload* evt, void* user_data);
typedef struct { LogosDeliveryOnReceivedMessageFn fn; void* user_data; } LogosDeliveryOnReceivedMessageBox;
static void logosdelivery_on_received_message_trampoline(int ret, const char* msg, size_t len, void* ud) {
    if (!ud || ret != 0 || !msg || len == 0) return;
    LogosDeliveryOnReceivedMessageBox* box = (LogosDeliveryOnReceivedMessageBox*)ud;
    if (!box->fn) return;
    CborParser parser;
    CborValue it;
    if (cbor_parser_init((const uint8_t*)msg, len, 0, &parser, &it) != CborNoError) return;
    if (!cbor_value_is_map(&it)) return;
    CborValue payloadField;
    if (cbor_value_map_find_value(&it, "payload", &payloadField) != CborNoError) return;
    ReceivedMessagePayload payload;
    memset(&payload, 0, sizeof(payload));
    if (logosdelivery_dec_ReceivedMessagePayload(&payloadField, &payload) != CborNoError) return;
    box->fn(&payload, box->user_data);
    logosdelivery_free_ReceivedMessagePayload(&payload);
}

/* ============================================================ */
/* High-level context wrapper                                   */
/* ============================================================ */
typedef struct {
    uint64_t id;
    void* box;
} LogosDeliveryCtxListener;

typedef struct {
    void* ptr;
    LogosDeliveryCtxListener* listeners;
    size_t listeners_len;
    size_t listeners_cap;
} LogosDeliveryCtx;

typedef void (*LogosDeliveryCreateFn)(int err_code, LogosDeliveryCtx* ctx, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryCreateFn fn; void* user_data; } LogosDeliveryCreateBox;
static void logosdelivery_create_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryCreateBox* box = (LogosDeliveryCreateBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI create failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr addr;
    memset(&addr, 0, sizeof(addr));
    if (nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &addr, &err) != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        free(box);
        return;
    }
    char* endp = NULL;
    unsigned long long a = addr.data ? strtoull(addr.data, &endp, 10) : 0;
    bool ok = addr.data && addr.len > 0 && endp && *endp == '\0';
    nimffi_free_str(&addr);
    if (!ok) {
        box->fn(-1, NULL, "FFI create returned non-numeric address", box->user_data);
        free(box);
        return;
    }
    LogosDeliveryCtx* ctx = (LogosDeliveryCtx*)calloc(1, sizeof(LogosDeliveryCtx));
    if (!ctx) {
        box->fn(-1, NULL, "out of memory", box->user_data);
        free(box);
        return;
    }
    ctx->ptr = (void*)(uintptr_t)a;
    box->fn(NIMFFI_RET_OK, ctx, NULL, box->user_data);
    free(box);
}

static inline int logosdelivery_ctx_create(NimFfiStr configJson, LogosDeliveryCreateFn on_created, void* user_data) {
    LogosdeliveryCreateNodeCtorReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.configJson = configJson;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryCreateNodeCtorReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_created) on_created(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryCreateBox* box = (LogosDeliveryCreateBox*)malloc(sizeof(LogosDeliveryCreateBox));
    if (!box) {
        free(req_buf);
        if (on_created) on_created(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_created;
    box->user_data = user_data;
    (void)logosdelivery_create_node(req_buf, req_len, logosdelivery_create_trampoline, box);
    free(req_buf);
    return 0;
}

static inline void logosdelivery_ctx_destroy(LogosDeliveryCtx* ctx) {
    if (!ctx) return;
    if (ctx->ptr) { logosdelivery_destroy(ctx->ptr); ctx->ptr = NULL; }
    for (size_t i = 0; i < ctx->listeners_len; i++) free(ctx->listeners[i].box);
    free(ctx->listeners);
    free(ctx);
}

static inline uint64_t logosdelivery_ctx_add_on_message_sent_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnMessageSentFn fn, void* user_data) {
    LogosDeliveryOnMessageSentBox* box = (LogosDeliveryOnMessageSentBox*)malloc(sizeof(LogosDeliveryOnMessageSentBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onMessageSent", logosdelivery_on_message_sent_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_message_error_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnMessageErrorFn fn, void* user_data) {
    LogosDeliveryOnMessageErrorBox* box = (LogosDeliveryOnMessageErrorBox*)malloc(sizeof(LogosDeliveryOnMessageErrorBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onMessageError", logosdelivery_on_message_error_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_message_propagated_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnMessagePropagatedFn fn, void* user_data) {
    LogosDeliveryOnMessagePropagatedBox* box = (LogosDeliveryOnMessagePropagatedBox*)malloc(sizeof(LogosDeliveryOnMessagePropagatedBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onMessagePropagated", logosdelivery_on_message_propagated_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_message_received_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnMessageReceivedFn fn, void* user_data) {
    LogosDeliveryOnMessageReceivedBox* box = (LogosDeliveryOnMessageReceivedBox*)malloc(sizeof(LogosDeliveryOnMessageReceivedBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onMessageReceived", logosdelivery_on_message_received_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_connection_status_change_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnConnectionStatusChangeFn fn, void* user_data) {
    LogosDeliveryOnConnectionStatusChangeBox* box = (LogosDeliveryOnConnectionStatusChangeBox*)malloc(sizeof(LogosDeliveryOnConnectionStatusChangeBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onConnectionStatusChange", logosdelivery_on_connection_status_change_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_topic_health_change_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnTopicHealthChangeFn fn, void* user_data) {
    LogosDeliveryOnTopicHealthChangeBox* box = (LogosDeliveryOnTopicHealthChangeBox*)malloc(sizeof(LogosDeliveryOnTopicHealthChangeBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onTopicHealthChange", logosdelivery_on_topic_health_change_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_connection_change_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnConnectionChangeFn fn, void* user_data) {
    LogosDeliveryOnConnectionChangeBox* box = (LogosDeliveryOnConnectionChangeBox*)malloc(sizeof(LogosDeliveryOnConnectionChangeBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onConnectionChange", logosdelivery_on_connection_change_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_channel_message_received_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnChannelMessageReceivedFn fn, void* user_data) {
    LogosDeliveryOnChannelMessageReceivedBox* box = (LogosDeliveryOnChannelMessageReceivedBox*)malloc(sizeof(LogosDeliveryOnChannelMessageReceivedBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onChannelMessageReceived", logosdelivery_on_channel_message_received_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_channel_message_sent_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnChannelMessageSentFn fn, void* user_data) {
    LogosDeliveryOnChannelMessageSentBox* box = (LogosDeliveryOnChannelMessageSentBox*)malloc(sizeof(LogosDeliveryOnChannelMessageSentBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onChannelMessageSent", logosdelivery_on_channel_message_sent_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_channel_message_error_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnChannelMessageErrorFn fn, void* user_data) {
    LogosDeliveryOnChannelMessageErrorBox* box = (LogosDeliveryOnChannelMessageErrorBox*)malloc(sizeof(LogosDeliveryOnChannelMessageErrorBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onChannelMessageError", logosdelivery_on_channel_message_error_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline uint64_t logosdelivery_ctx_add_on_received_message_listener(LogosDeliveryCtx* ctx, LogosDeliveryOnReceivedMessageFn fn, void* user_data) {
    LogosDeliveryOnReceivedMessageBox* box = (LogosDeliveryOnReceivedMessageBox*)malloc(sizeof(LogosDeliveryOnReceivedMessageBox));
    if (!box) return 0;
    box->fn = fn;
    box->user_data = user_data;
    uint64_t id = logosdelivery_add_event_listener(ctx->ptr, "onReceivedMessage", logosdelivery_on_received_message_trampoline, box);
    if (id == 0) { free(box); return 0; }
    if (ctx->listeners_len == ctx->listeners_cap) {
        size_t ncap = ctx->listeners_cap ? ctx->listeners_cap * 2 : 4;
        LogosDeliveryCtxListener* grown = (LogosDeliveryCtxListener*)realloc(ctx->listeners, ncap * sizeof(LogosDeliveryCtxListener));
        if (!grown) { logosdelivery_remove_event_listener(ctx->ptr, id); free(box); return 0; }
        ctx->listeners = grown;
        ctx->listeners_cap = ncap;
    }
    ctx->listeners[ctx->listeners_len].id = id;
    ctx->listeners[ctx->listeners_len].box = box;
    ctx->listeners_len++;
    return id;
}

static inline bool logosdelivery_ctx_remove_event_listener(LogosDeliveryCtx* ctx, uint64_t id) {
    if (id == 0) return false;
    int rc = logosdelivery_remove_event_listener(ctx->ptr, id);
    for (size_t i = 0; i < ctx->listeners_len; i++) {
        if (ctx->listeners[i].id == id) {
            free(ctx->listeners[i].box);
            ctx->listeners[i] = ctx->listeners[ctx->listeners_len - 1];
            ctx->listeners_len--;
            break;
        }
    }
    return rc == 0;
}

typedef void (*LogosDeliveryStartNodeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryStartNodeReplyFn fn; void* user_data; } LogosDeliveryStartNodeCallBox;
static void logosdelivery_start_node_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryStartNodeCallBox* box = (LogosDeliveryStartNodeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_start_node(const LogosDeliveryCtx* ctx, LogosDeliveryStartNodeReplyFn on_reply, void* user_data) {
    LogosdeliveryStartNodeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryStartNodeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryStartNodeCallBox* box = (LogosDeliveryStartNodeCallBox*)malloc(sizeof(LogosDeliveryStartNodeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_start_node(ctx->ptr, logosdelivery_start_node_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryStopNodeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryStopNodeReplyFn fn; void* user_data; } LogosDeliveryStopNodeCallBox;
static void logosdelivery_stop_node_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryStopNodeCallBox* box = (LogosDeliveryStopNodeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_stop_node(const LogosDeliveryCtx* ctx, LogosDeliveryStopNodeReplyFn on_reply, void* user_data) {
    LogosdeliveryStopNodeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryStopNodeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryStopNodeCallBox* box = (LogosDeliveryStopNodeCallBox*)malloc(sizeof(LogosDeliveryStopNodeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_stop_node(ctx->ptr, logosdelivery_stop_node_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliverySubscribeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliverySubscribeReplyFn fn; void* user_data; } LogosDeliverySubscribeCallBox;
static void logosdelivery_subscribe_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliverySubscribeCallBox* box = (LogosDeliverySubscribeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_subscribe(const LogosDeliveryCtx* ctx, NimFfiStr contentTopicStr, LogosDeliverySubscribeReplyFn on_reply, void* user_data) {
    LogosdeliverySubscribeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.contentTopicStr = contentTopicStr;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliverySubscribeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliverySubscribeCallBox* box = (LogosDeliverySubscribeCallBox*)malloc(sizeof(LogosDeliverySubscribeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_subscribe(ctx->ptr, logosdelivery_subscribe_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryUnsubscribeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryUnsubscribeReplyFn fn; void* user_data; } LogosDeliveryUnsubscribeCallBox;
static void logosdelivery_unsubscribe_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryUnsubscribeCallBox* box = (LogosDeliveryUnsubscribeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_unsubscribe(const LogosDeliveryCtx* ctx, NimFfiStr contentTopicStr, LogosDeliveryUnsubscribeReplyFn on_reply, void* user_data) {
    LogosdeliveryUnsubscribeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.contentTopicStr = contentTopicStr;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryUnsubscribeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryUnsubscribeCallBox* box = (LogosDeliveryUnsubscribeCallBox*)malloc(sizeof(LogosDeliveryUnsubscribeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_unsubscribe(ctx->ptr, logosdelivery_unsubscribe_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliverySendReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliverySendReplyFn fn; void* user_data; } LogosDeliverySendCallBox;
static void logosdelivery_send_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliverySendCallBox* box = (LogosDeliverySendCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_send(const LogosDeliveryCtx* ctx, const SendRequest* req, LogosDeliverySendReplyFn on_reply, void* user_data) {
    LogosdeliverySendReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.req = *req;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliverySendReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliverySendCallBox* box = (LogosDeliverySendCallBox*)malloc(sizeof(LogosDeliverySendCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_send(ctx->ptr, logosdelivery_send_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryGetAvailableNodeInfoIdsReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryGetAvailableNodeInfoIdsReplyFn fn; void* user_data; } LogosDeliveryGetAvailableNodeInfoIdsCallBox;
static void logosdelivery_get_available_node_info_ids_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryGetAvailableNodeInfoIdsCallBox* box = (LogosDeliveryGetAvailableNodeInfoIdsCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_get_available_node_info_ids(const LogosDeliveryCtx* ctx, LogosDeliveryGetAvailableNodeInfoIdsReplyFn on_reply, void* user_data) {
    LogosdeliveryGetAvailableNodeInfoIdsReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryGetAvailableNodeInfoIdsReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryGetAvailableNodeInfoIdsCallBox* box = (LogosDeliveryGetAvailableNodeInfoIdsCallBox*)malloc(sizeof(LogosDeliveryGetAvailableNodeInfoIdsCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_get_available_node_info_ids(ctx->ptr, logosdelivery_get_available_node_info_ids_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryGetNodeInfoReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryGetNodeInfoReplyFn fn; void* user_data; } LogosDeliveryGetNodeInfoCallBox;
static void logosdelivery_get_node_info_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryGetNodeInfoCallBox* box = (LogosDeliveryGetNodeInfoCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_get_node_info(const LogosDeliveryCtx* ctx, NimFfiStr nodeInfoId, LogosDeliveryGetNodeInfoReplyFn on_reply, void* user_data) {
    LogosdeliveryGetNodeInfoReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.nodeInfoId = nodeInfoId;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryGetNodeInfoReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryGetNodeInfoCallBox* box = (LogosDeliveryGetNodeInfoCallBox*)malloc(sizeof(LogosDeliveryGetNodeInfoCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_get_node_info(ctx->ptr, logosdelivery_get_node_info_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryGetAvailableConfigsReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryGetAvailableConfigsReplyFn fn; void* user_data; } LogosDeliveryGetAvailableConfigsCallBox;
static void logosdelivery_get_available_configs_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryGetAvailableConfigsCallBox* box = (LogosDeliveryGetAvailableConfigsCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_get_available_configs(const LogosDeliveryCtx* ctx, LogosDeliveryGetAvailableConfigsReplyFn on_reply, void* user_data) {
    LogosdeliveryGetAvailableConfigsReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryGetAvailableConfigsReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryGetAvailableConfigsCallBox* box = (LogosDeliveryGetAvailableConfigsCallBox*)malloc(sizeof(LogosDeliveryGetAvailableConfigsCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_get_available_configs(ctx->ptr, logosdelivery_get_available_configs_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetPeeridsFromPeerstoreReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetPeeridsFromPeerstoreReplyFn fn; void* user_data; } LogosDeliveryWakuGetPeeridsFromPeerstoreCallBox;
static void logosdelivery_waku_get_peerids_from_peerstore_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetPeeridsFromPeerstoreCallBox* box = (LogosDeliveryWakuGetPeeridsFromPeerstoreCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_peerids_from_peerstore(const LogosDeliveryCtx* ctx, LogosDeliveryWakuGetPeeridsFromPeerstoreReplyFn on_reply, void* user_data) {
    WakuGetPeeridsFromPeerstoreReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetPeeridsFromPeerstoreReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetPeeridsFromPeerstoreCallBox* box = (LogosDeliveryWakuGetPeeridsFromPeerstoreCallBox*)malloc(sizeof(LogosDeliveryWakuGetPeeridsFromPeerstoreCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_peerids_from_peerstore(ctx->ptr, logosdelivery_waku_get_peerids_from_peerstore_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuConnectReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuConnectReplyFn fn; void* user_data; } LogosDeliveryWakuConnectCallBox;
static void logosdelivery_waku_connect_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuConnectCallBox* box = (LogosDeliveryWakuConnectCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_connect(const LogosDeliveryCtx* ctx, NimFfiStr peerMultiAddr, uint32_t timeoutMs, LogosDeliveryWakuConnectReplyFn on_reply, void* user_data) {
    WakuConnectReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.peerMultiAddr = peerMultiAddr;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuConnectReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuConnectCallBox* box = (LogosDeliveryWakuConnectCallBox*)malloc(sizeof(LogosDeliveryWakuConnectCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_connect(ctx->ptr, logosdelivery_waku_connect_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDisconnectPeerByIdReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDisconnectPeerByIdReplyFn fn; void* user_data; } LogosDeliveryWakuDisconnectPeerByIdCallBox;
static void logosdelivery_waku_disconnect_peer_by_id_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDisconnectPeerByIdCallBox* box = (LogosDeliveryWakuDisconnectPeerByIdCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_disconnect_peer_by_id(const LogosDeliveryCtx* ctx, NimFfiStr peerId, LogosDeliveryWakuDisconnectPeerByIdReplyFn on_reply, void* user_data) {
    WakuDisconnectPeerByIdReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.peerId = peerId;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDisconnectPeerByIdReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDisconnectPeerByIdCallBox* box = (LogosDeliveryWakuDisconnectPeerByIdCallBox*)malloc(sizeof(LogosDeliveryWakuDisconnectPeerByIdCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_disconnect_peer_by_id(ctx->ptr, logosdelivery_waku_disconnect_peer_by_id_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDisconnectAllPeersReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDisconnectAllPeersReplyFn fn; void* user_data; } LogosDeliveryWakuDisconnectAllPeersCallBox;
static void logosdelivery_waku_disconnect_all_peers_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDisconnectAllPeersCallBox* box = (LogosDeliveryWakuDisconnectAllPeersCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_disconnect_all_peers(const LogosDeliveryCtx* ctx, LogosDeliveryWakuDisconnectAllPeersReplyFn on_reply, void* user_data) {
    WakuDisconnectAllPeersReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDisconnectAllPeersReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDisconnectAllPeersCallBox* box = (LogosDeliveryWakuDisconnectAllPeersCallBox*)malloc(sizeof(LogosDeliveryWakuDisconnectAllPeersCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_disconnect_all_peers(ctx->ptr, logosdelivery_waku_disconnect_all_peers_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDialPeerReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDialPeerReplyFn fn; void* user_data; } LogosDeliveryWakuDialPeerCallBox;
static void logosdelivery_waku_dial_peer_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDialPeerCallBox* box = (LogosDeliveryWakuDialPeerCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_dial_peer(const LogosDeliveryCtx* ctx, NimFfiStr peerMultiAddr, NimFfiStr protocol, uint32_t timeoutMs, LogosDeliveryWakuDialPeerReplyFn on_reply, void* user_data) {
    WakuDialPeerReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.peerMultiAddr = peerMultiAddr;
    ffi_req.protocol = protocol;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDialPeerReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDialPeerCallBox* box = (LogosDeliveryWakuDialPeerCallBox*)malloc(sizeof(LogosDeliveryWakuDialPeerCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_dial_peer(ctx->ptr, logosdelivery_waku_dial_peer_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDialPeerByIdReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDialPeerByIdReplyFn fn; void* user_data; } LogosDeliveryWakuDialPeerByIdCallBox;
static void logosdelivery_waku_dial_peer_by_id_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDialPeerByIdCallBox* box = (LogosDeliveryWakuDialPeerByIdCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_dial_peer_by_id(const LogosDeliveryCtx* ctx, NimFfiStr peerId, NimFfiStr protocol, uint32_t timeoutMs, LogosDeliveryWakuDialPeerByIdReplyFn on_reply, void* user_data) {
    WakuDialPeerByIdReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.peerId = peerId;
    ffi_req.protocol = protocol;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDialPeerByIdReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDialPeerByIdCallBox* box = (LogosDeliveryWakuDialPeerByIdCallBox*)malloc(sizeof(LogosDeliveryWakuDialPeerByIdCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_dial_peer_by_id(ctx->ptr, logosdelivery_waku_dial_peer_by_id_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetConnectedPeersInfoReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetConnectedPeersInfoReplyFn fn; void* user_data; } LogosDeliveryWakuGetConnectedPeersInfoCallBox;
static void logosdelivery_waku_get_connected_peers_info_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetConnectedPeersInfoCallBox* box = (LogosDeliveryWakuGetConnectedPeersInfoCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_connected_peers_info(const LogosDeliveryCtx* ctx, LogosDeliveryWakuGetConnectedPeersInfoReplyFn on_reply, void* user_data) {
    WakuGetConnectedPeersInfoReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetConnectedPeersInfoReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetConnectedPeersInfoCallBox* box = (LogosDeliveryWakuGetConnectedPeersInfoCallBox*)malloc(sizeof(LogosDeliveryWakuGetConnectedPeersInfoCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_connected_peers_info(ctx->ptr, logosdelivery_waku_get_connected_peers_info_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetConnectedPeersReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetConnectedPeersReplyFn fn; void* user_data; } LogosDeliveryWakuGetConnectedPeersCallBox;
static void logosdelivery_waku_get_connected_peers_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetConnectedPeersCallBox* box = (LogosDeliveryWakuGetConnectedPeersCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_connected_peers(const LogosDeliveryCtx* ctx, LogosDeliveryWakuGetConnectedPeersReplyFn on_reply, void* user_data) {
    WakuGetConnectedPeersReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetConnectedPeersReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetConnectedPeersCallBox* box = (LogosDeliveryWakuGetConnectedPeersCallBox*)malloc(sizeof(LogosDeliveryWakuGetConnectedPeersCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_connected_peers(ctx->ptr, logosdelivery_waku_get_connected_peers_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetPeeridsByProtocolReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetPeeridsByProtocolReplyFn fn; void* user_data; } LogosDeliveryWakuGetPeeridsByProtocolCallBox;
static void logosdelivery_waku_get_peerids_by_protocol_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetPeeridsByProtocolCallBox* box = (LogosDeliveryWakuGetPeeridsByProtocolCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_peerids_by_protocol(const LogosDeliveryCtx* ctx, NimFfiStr protocol, LogosDeliveryWakuGetPeeridsByProtocolReplyFn on_reply, void* user_data) {
    WakuGetPeeridsByProtocolReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.protocol = protocol;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetPeeridsByProtocolReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetPeeridsByProtocolCallBox* box = (LogosDeliveryWakuGetPeeridsByProtocolCallBox*)malloc(sizeof(LogosDeliveryWakuGetPeeridsByProtocolCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_peerids_by_protocol(ctx->ptr, logosdelivery_waku_get_peerids_by_protocol_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDiscv5UpdateBootnodesReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDiscv5UpdateBootnodesReplyFn fn; void* user_data; } LogosDeliveryWakuDiscv5UpdateBootnodesCallBox;
static void logosdelivery_waku_discv5_update_bootnodes_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDiscv5UpdateBootnodesCallBox* box = (LogosDeliveryWakuDiscv5UpdateBootnodesCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_discv5_update_bootnodes(const LogosDeliveryCtx* ctx, NimFfiStr bootnodes, LogosDeliveryWakuDiscv5UpdateBootnodesReplyFn on_reply, void* user_data) {
    WakuDiscv5UpdateBootnodesReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.bootnodes = bootnodes;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDiscv5UpdateBootnodesReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDiscv5UpdateBootnodesCallBox* box = (LogosDeliveryWakuDiscv5UpdateBootnodesCallBox*)malloc(sizeof(LogosDeliveryWakuDiscv5UpdateBootnodesCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_discv5_update_bootnodes(ctx->ptr, logosdelivery_waku_discv5_update_bootnodes_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDnsDiscoveryReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDnsDiscoveryReplyFn fn; void* user_data; } LogosDeliveryWakuDnsDiscoveryCallBox;
static void logosdelivery_waku_dns_discovery_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDnsDiscoveryCallBox* box = (LogosDeliveryWakuDnsDiscoveryCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_dns_discovery(const LogosDeliveryCtx* ctx, NimFfiStr enrTreeUrl, NimFfiStr nameDnsServer, int32_t timeoutMs, LogosDeliveryWakuDnsDiscoveryReplyFn on_reply, void* user_data) {
    WakuDnsDiscoveryReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.enrTreeUrl = enrTreeUrl;
    ffi_req.nameDnsServer = nameDnsServer;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDnsDiscoveryReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDnsDiscoveryCallBox* box = (LogosDeliveryWakuDnsDiscoveryCallBox*)malloc(sizeof(LogosDeliveryWakuDnsDiscoveryCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_dns_discovery(ctx->ptr, logosdelivery_waku_dns_discovery_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuStartDiscv5ReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuStartDiscv5ReplyFn fn; void* user_data; } LogosDeliveryWakuStartDiscv5CallBox;
static void logosdelivery_waku_start_discv5_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuStartDiscv5CallBox* box = (LogosDeliveryWakuStartDiscv5CallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_start_discv5(const LogosDeliveryCtx* ctx, LogosDeliveryWakuStartDiscv5ReplyFn on_reply, void* user_data) {
    WakuStartDiscv5Req ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuStartDiscv5Req, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuStartDiscv5CallBox* box = (LogosDeliveryWakuStartDiscv5CallBox*)malloc(sizeof(LogosDeliveryWakuStartDiscv5CallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_start_discv5(ctx->ptr, logosdelivery_waku_start_discv5_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuStopDiscv5ReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuStopDiscv5ReplyFn fn; void* user_data; } LogosDeliveryWakuStopDiscv5CallBox;
static void logosdelivery_waku_stop_discv5_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuStopDiscv5CallBox* box = (LogosDeliveryWakuStopDiscv5CallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_stop_discv5(const LogosDeliveryCtx* ctx, LogosDeliveryWakuStopDiscv5ReplyFn on_reply, void* user_data) {
    WakuStopDiscv5Req ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuStopDiscv5Req, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuStopDiscv5CallBox* box = (LogosDeliveryWakuStopDiscv5CallBox*)malloc(sizeof(LogosDeliveryWakuStopDiscv5CallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_stop_discv5(ctx->ptr, logosdelivery_waku_stop_discv5_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuPeerExchangeRequestReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuPeerExchangeRequestReplyFn fn; void* user_data; } LogosDeliveryWakuPeerExchangeRequestCallBox;
static void logosdelivery_waku_peer_exchange_request_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuPeerExchangeRequestCallBox* box = (LogosDeliveryWakuPeerExchangeRequestCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_peer_exchange_request(const LogosDeliveryCtx* ctx, uint64_t numPeers, LogosDeliveryWakuPeerExchangeRequestReplyFn on_reply, void* user_data) {
    WakuPeerExchangeRequestReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.numPeers = numPeers;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuPeerExchangeRequestReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuPeerExchangeRequestCallBox* box = (LogosDeliveryWakuPeerExchangeRequestCallBox*)malloc(sizeof(LogosDeliveryWakuPeerExchangeRequestCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_peer_exchange_request(ctx->ptr, logosdelivery_waku_peer_exchange_request_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuVersionReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuVersionReplyFn fn; void* user_data; } LogosDeliveryWakuVersionCallBox;
static void logosdelivery_waku_version_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuVersionCallBox* box = (LogosDeliveryWakuVersionCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_version(const LogosDeliveryCtx* ctx, LogosDeliveryWakuVersionReplyFn on_reply, void* user_data) {
    WakuVersionReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuVersionReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuVersionCallBox* box = (LogosDeliveryWakuVersionCallBox*)malloc(sizeof(LogosDeliveryWakuVersionCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_version(ctx->ptr, logosdelivery_waku_version_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuListenAddressesReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuListenAddressesReplyFn fn; void* user_data; } LogosDeliveryWakuListenAddressesCallBox;
static void logosdelivery_waku_listen_addresses_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuListenAddressesCallBox* box = (LogosDeliveryWakuListenAddressesCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_listen_addresses(const LogosDeliveryCtx* ctx, LogosDeliveryWakuListenAddressesReplyFn on_reply, void* user_data) {
    WakuListenAddressesReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuListenAddressesReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuListenAddressesCallBox* box = (LogosDeliveryWakuListenAddressesCallBox*)malloc(sizeof(LogosDeliveryWakuListenAddressesCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_listen_addresses(ctx->ptr, logosdelivery_waku_listen_addresses_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetMyEnrReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetMyEnrReplyFn fn; void* user_data; } LogosDeliveryWakuGetMyEnrCallBox;
static void logosdelivery_waku_get_my_enr_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetMyEnrCallBox* box = (LogosDeliveryWakuGetMyEnrCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_my_enr(const LogosDeliveryCtx* ctx, LogosDeliveryWakuGetMyEnrReplyFn on_reply, void* user_data) {
    WakuGetMyEnrReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetMyEnrReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetMyEnrCallBox* box = (LogosDeliveryWakuGetMyEnrCallBox*)malloc(sizeof(LogosDeliveryWakuGetMyEnrCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_my_enr(ctx->ptr, logosdelivery_waku_get_my_enr_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetMyPeeridReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetMyPeeridReplyFn fn; void* user_data; } LogosDeliveryWakuGetMyPeeridCallBox;
static void logosdelivery_waku_get_my_peerid_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetMyPeeridCallBox* box = (LogosDeliveryWakuGetMyPeeridCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_my_peerid(const LogosDeliveryCtx* ctx, LogosDeliveryWakuGetMyPeeridReplyFn on_reply, void* user_data) {
    WakuGetMyPeeridReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetMyPeeridReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetMyPeeridCallBox* box = (LogosDeliveryWakuGetMyPeeridCallBox*)malloc(sizeof(LogosDeliveryWakuGetMyPeeridCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_my_peerid(ctx->ptr, logosdelivery_waku_get_my_peerid_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuGetMetricsReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuGetMetricsReplyFn fn; void* user_data; } LogosDeliveryWakuGetMetricsCallBox;
static void logosdelivery_waku_get_metrics_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuGetMetricsCallBox* box = (LogosDeliveryWakuGetMetricsCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_get_metrics(const LogosDeliveryCtx* ctx, LogosDeliveryWakuGetMetricsReplyFn on_reply, void* user_data) {
    WakuGetMetricsReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuGetMetricsReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuGetMetricsCallBox* box = (LogosDeliveryWakuGetMetricsCallBox*)malloc(sizeof(LogosDeliveryWakuGetMetricsCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_get_metrics(ctx->ptr, logosdelivery_waku_get_metrics_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuIsOnlineReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuIsOnlineReplyFn fn; void* user_data; } LogosDeliveryWakuIsOnlineCallBox;
static void logosdelivery_waku_is_online_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuIsOnlineCallBox* box = (LogosDeliveryWakuIsOnlineCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_is_online(const LogosDeliveryCtx* ctx, LogosDeliveryWakuIsOnlineReplyFn on_reply, void* user_data) {
    WakuIsOnlineReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuIsOnlineReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuIsOnlineCallBox* box = (LogosDeliveryWakuIsOnlineCallBox*)malloc(sizeof(LogosDeliveryWakuIsOnlineCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_is_online(ctx->ptr, logosdelivery_waku_is_online_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuPingPeerReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuPingPeerReplyFn fn; void* user_data; } LogosDeliveryWakuPingPeerCallBox;
static void logosdelivery_waku_ping_peer_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuPingPeerCallBox* box = (LogosDeliveryWakuPingPeerCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_ping_peer(const LogosDeliveryCtx* ctx, NimFfiStr peerAddr, uint32_t timeoutMs, LogosDeliveryWakuPingPeerReplyFn on_reply, void* user_data) {
    WakuPingPeerReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.peerAddr = peerAddr;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuPingPeerReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuPingPeerCallBox* box = (LogosDeliveryWakuPingPeerCallBox*)malloc(sizeof(LogosDeliveryWakuPingPeerCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_ping_peer(ctx->ptr, logosdelivery_waku_ping_peer_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayGetPeersInMeshReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayGetPeersInMeshReplyFn fn; void* user_data; } LogosDeliveryWakuRelayGetPeersInMeshCallBox;
static void logosdelivery_waku_relay_get_peers_in_mesh_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayGetPeersInMeshCallBox* box = (LogosDeliveryWakuRelayGetPeersInMeshCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_get_peers_in_mesh(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, LogosDeliveryWakuRelayGetPeersInMeshReplyFn on_reply, void* user_data) {
    WakuRelayGetPeersInMeshReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayGetPeersInMeshReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayGetPeersInMeshCallBox* box = (LogosDeliveryWakuRelayGetPeersInMeshCallBox*)malloc(sizeof(LogosDeliveryWakuRelayGetPeersInMeshCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_get_peers_in_mesh(ctx->ptr, logosdelivery_waku_relay_get_peers_in_mesh_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayGetNumPeersInMeshReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayGetNumPeersInMeshReplyFn fn; void* user_data; } LogosDeliveryWakuRelayGetNumPeersInMeshCallBox;
static void logosdelivery_waku_relay_get_num_peers_in_mesh_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayGetNumPeersInMeshCallBox* box = (LogosDeliveryWakuRelayGetNumPeersInMeshCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_get_num_peers_in_mesh(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, LogosDeliveryWakuRelayGetNumPeersInMeshReplyFn on_reply, void* user_data) {
    WakuRelayGetNumPeersInMeshReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayGetNumPeersInMeshReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayGetNumPeersInMeshCallBox* box = (LogosDeliveryWakuRelayGetNumPeersInMeshCallBox*)malloc(sizeof(LogosDeliveryWakuRelayGetNumPeersInMeshCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_get_num_peers_in_mesh(ctx->ptr, logosdelivery_waku_relay_get_num_peers_in_mesh_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayGetConnectedPeersReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayGetConnectedPeersReplyFn fn; void* user_data; } LogosDeliveryWakuRelayGetConnectedPeersCallBox;
static void logosdelivery_waku_relay_get_connected_peers_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayGetConnectedPeersCallBox* box = (LogosDeliveryWakuRelayGetConnectedPeersCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_get_connected_peers(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, LogosDeliveryWakuRelayGetConnectedPeersReplyFn on_reply, void* user_data) {
    WakuRelayGetConnectedPeersReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayGetConnectedPeersReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayGetConnectedPeersCallBox* box = (LogosDeliveryWakuRelayGetConnectedPeersCallBox*)malloc(sizeof(LogosDeliveryWakuRelayGetConnectedPeersCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_get_connected_peers(ctx->ptr, logosdelivery_waku_relay_get_connected_peers_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayGetNumConnectedPeersReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayGetNumConnectedPeersReplyFn fn; void* user_data; } LogosDeliveryWakuRelayGetNumConnectedPeersCallBox;
static void logosdelivery_waku_relay_get_num_connected_peers_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayGetNumConnectedPeersCallBox* box = (LogosDeliveryWakuRelayGetNumConnectedPeersCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_get_num_connected_peers(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, LogosDeliveryWakuRelayGetNumConnectedPeersReplyFn on_reply, void* user_data) {
    WakuRelayGetNumConnectedPeersReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayGetNumConnectedPeersReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayGetNumConnectedPeersCallBox* box = (LogosDeliveryWakuRelayGetNumConnectedPeersCallBox*)malloc(sizeof(LogosDeliveryWakuRelayGetNumConnectedPeersCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_get_num_connected_peers(ctx->ptr, logosdelivery_waku_relay_get_num_connected_peers_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayAddProtectedShardReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayAddProtectedShardReplyFn fn; void* user_data; } LogosDeliveryWakuRelayAddProtectedShardCallBox;
static void logosdelivery_waku_relay_add_protected_shard_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayAddProtectedShardCallBox* box = (LogosDeliveryWakuRelayAddProtectedShardCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_add_protected_shard(const LogosDeliveryCtx* ctx, int32_t clusterId, int32_t shardId, NimFfiStr publicKey, LogosDeliveryWakuRelayAddProtectedShardReplyFn on_reply, void* user_data) {
    WakuRelayAddProtectedShardReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.clusterId = clusterId;
    ffi_req.shardId = shardId;
    ffi_req.publicKey = publicKey;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayAddProtectedShardReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayAddProtectedShardCallBox* box = (LogosDeliveryWakuRelayAddProtectedShardCallBox*)malloc(sizeof(LogosDeliveryWakuRelayAddProtectedShardCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_add_protected_shard(ctx->ptr, logosdelivery_waku_relay_add_protected_shard_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelaySubscribeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelaySubscribeReplyFn fn; void* user_data; } LogosDeliveryWakuRelaySubscribeCallBox;
static void logosdelivery_waku_relay_subscribe_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelaySubscribeCallBox* box = (LogosDeliveryWakuRelaySubscribeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_subscribe(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, LogosDeliveryWakuRelaySubscribeReplyFn on_reply, void* user_data) {
    WakuRelaySubscribeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelaySubscribeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelaySubscribeCallBox* box = (LogosDeliveryWakuRelaySubscribeCallBox*)malloc(sizeof(LogosDeliveryWakuRelaySubscribeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_subscribe(ctx->ptr, logosdelivery_waku_relay_subscribe_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayUnsubscribeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayUnsubscribeReplyFn fn; void* user_data; } LogosDeliveryWakuRelayUnsubscribeCallBox;
static void logosdelivery_waku_relay_unsubscribe_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayUnsubscribeCallBox* box = (LogosDeliveryWakuRelayUnsubscribeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_unsubscribe(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, LogosDeliveryWakuRelayUnsubscribeReplyFn on_reply, void* user_data) {
    WakuRelayUnsubscribeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayUnsubscribeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayUnsubscribeCallBox* box = (LogosDeliveryWakuRelayUnsubscribeCallBox*)malloc(sizeof(LogosDeliveryWakuRelayUnsubscribeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_unsubscribe(ctx->ptr, logosdelivery_waku_relay_unsubscribe_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuRelayPublishReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuRelayPublishReplyFn fn; void* user_data; } LogosDeliveryWakuRelayPublishCallBox;
static void logosdelivery_waku_relay_publish_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuRelayPublishCallBox* box = (LogosDeliveryWakuRelayPublishCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_relay_publish(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, NimFfiStr jsonWakuMessage, uint32_t timeoutMs, LogosDeliveryWakuRelayPublishReplyFn on_reply, void* user_data) {
    WakuRelayPublishReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    ffi_req.jsonWakuMessage = jsonWakuMessage;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuRelayPublishReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuRelayPublishCallBox* box = (LogosDeliveryWakuRelayPublishCallBox*)malloc(sizeof(LogosDeliveryWakuRelayPublishCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_relay_publish(ctx->ptr, logosdelivery_waku_relay_publish_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuDefaultPubsubTopicReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuDefaultPubsubTopicReplyFn fn; void* user_data; } LogosDeliveryWakuDefaultPubsubTopicCallBox;
static void logosdelivery_waku_default_pubsub_topic_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuDefaultPubsubTopicCallBox* box = (LogosDeliveryWakuDefaultPubsubTopicCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_default_pubsub_topic(const LogosDeliveryCtx* ctx, LogosDeliveryWakuDefaultPubsubTopicReplyFn on_reply, void* user_data) {
    WakuDefaultPubsubTopicReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuDefaultPubsubTopicReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuDefaultPubsubTopicCallBox* box = (LogosDeliveryWakuDefaultPubsubTopicCallBox*)malloc(sizeof(LogosDeliveryWakuDefaultPubsubTopicCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_default_pubsub_topic(ctx->ptr, logosdelivery_waku_default_pubsub_topic_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuContentTopicReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuContentTopicReplyFn fn; void* user_data; } LogosDeliveryWakuContentTopicCallBox;
static void logosdelivery_waku_content_topic_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuContentTopicCallBox* box = (LogosDeliveryWakuContentTopicCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_content_topic(const LogosDeliveryCtx* ctx, NimFfiStr appName, uint32_t appVersion, NimFfiStr contentTopicName, NimFfiStr encoding, LogosDeliveryWakuContentTopicReplyFn on_reply, void* user_data) {
    WakuContentTopicReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.appName = appName;
    ffi_req.appVersion = appVersion;
    ffi_req.contentTopicName = contentTopicName;
    ffi_req.encoding = encoding;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuContentTopicReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuContentTopicCallBox* box = (LogosDeliveryWakuContentTopicCallBox*)malloc(sizeof(LogosDeliveryWakuContentTopicCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_content_topic(ctx->ptr, logosdelivery_waku_content_topic_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuPubsubTopicReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuPubsubTopicReplyFn fn; void* user_data; } LogosDeliveryWakuPubsubTopicCallBox;
static void logosdelivery_waku_pubsub_topic_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuPubsubTopicCallBox* box = (LogosDeliveryWakuPubsubTopicCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_pubsub_topic(const LogosDeliveryCtx* ctx, NimFfiStr topicName, LogosDeliveryWakuPubsubTopicReplyFn on_reply, void* user_data) {
    WakuPubsubTopicReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.topicName = topicName;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuPubsubTopicReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuPubsubTopicCallBox* box = (LogosDeliveryWakuPubsubTopicCallBox*)malloc(sizeof(LogosDeliveryWakuPubsubTopicCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_pubsub_topic(ctx->ptr, logosdelivery_waku_pubsub_topic_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuStoreQueryReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuStoreQueryReplyFn fn; void* user_data; } LogosDeliveryWakuStoreQueryCallBox;
static void logosdelivery_waku_store_query_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuStoreQueryCallBox* box = (LogosDeliveryWakuStoreQueryCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_store_query(const LogosDeliveryCtx* ctx, NimFfiStr jsonQuery, NimFfiStr peerAddr, int32_t timeoutMs, LogosDeliveryWakuStoreQueryReplyFn on_reply, void* user_data) {
    WakuStoreQueryReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.jsonQuery = jsonQuery;
    ffi_req.peerAddr = peerAddr;
    ffi_req.timeoutMs = timeoutMs;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuStoreQueryReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuStoreQueryCallBox* box = (LogosDeliveryWakuStoreQueryCallBox*)malloc(sizeof(LogosDeliveryWakuStoreQueryCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_store_query(ctx->ptr, logosdelivery_waku_store_query_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuLightpushPublishReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuLightpushPublishReplyFn fn; void* user_data; } LogosDeliveryWakuLightpushPublishCallBox;
static void logosdelivery_waku_lightpush_publish_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuLightpushPublishCallBox* box = (LogosDeliveryWakuLightpushPublishCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_lightpush_publish(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, NimFfiStr jsonWakuMessage, LogosDeliveryWakuLightpushPublishReplyFn on_reply, void* user_data) {
    WakuLightpushPublishReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    ffi_req.jsonWakuMessage = jsonWakuMessage;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuLightpushPublishReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuLightpushPublishCallBox* box = (LogosDeliveryWakuLightpushPublishCallBox*)malloc(sizeof(LogosDeliveryWakuLightpushPublishCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_lightpush_publish(ctx->ptr, logosdelivery_waku_lightpush_publish_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuFilterSubscribeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuFilterSubscribeReplyFn fn; void* user_data; } LogosDeliveryWakuFilterSubscribeCallBox;
static void logosdelivery_waku_filter_subscribe_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuFilterSubscribeCallBox* box = (LogosDeliveryWakuFilterSubscribeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_filter_subscribe(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, NimFfiStr contentTopics, LogosDeliveryWakuFilterSubscribeReplyFn on_reply, void* user_data) {
    WakuFilterSubscribeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    ffi_req.contentTopics = contentTopics;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuFilterSubscribeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuFilterSubscribeCallBox* box = (LogosDeliveryWakuFilterSubscribeCallBox*)malloc(sizeof(LogosDeliveryWakuFilterSubscribeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_filter_subscribe(ctx->ptr, logosdelivery_waku_filter_subscribe_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuFilterUnsubscribeReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuFilterUnsubscribeReplyFn fn; void* user_data; } LogosDeliveryWakuFilterUnsubscribeCallBox;
static void logosdelivery_waku_filter_unsubscribe_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuFilterUnsubscribeCallBox* box = (LogosDeliveryWakuFilterUnsubscribeCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_filter_unsubscribe(const LogosDeliveryCtx* ctx, NimFfiStr pubSubTopic, NimFfiStr contentTopics, LogosDeliveryWakuFilterUnsubscribeReplyFn on_reply, void* user_data) {
    WakuFilterUnsubscribeReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.pubSubTopic = pubSubTopic;
    ffi_req.contentTopics = contentTopics;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuFilterUnsubscribeReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuFilterUnsubscribeCallBox* box = (LogosDeliveryWakuFilterUnsubscribeCallBox*)malloc(sizeof(LogosDeliveryWakuFilterUnsubscribeCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_filter_unsubscribe(ctx->ptr, logosdelivery_waku_filter_unsubscribe_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryWakuFilterUnsubscribeAllReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryWakuFilterUnsubscribeAllReplyFn fn; void* user_data; } LogosDeliveryWakuFilterUnsubscribeAllCallBox;
static void logosdelivery_waku_filter_unsubscribe_all_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryWakuFilterUnsubscribeAllCallBox* box = (LogosDeliveryWakuFilterUnsubscribeAllCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_waku_filter_unsubscribe_all(const LogosDeliveryCtx* ctx, LogosDeliveryWakuFilterUnsubscribeAllReplyFn on_reply, void* user_data) {
    WakuFilterUnsubscribeAllReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_WakuFilterUnsubscribeAllReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryWakuFilterUnsubscribeAllCallBox* box = (LogosDeliveryWakuFilterUnsubscribeAllCallBox*)malloc(sizeof(LogosDeliveryWakuFilterUnsubscribeAllCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = waku_filter_unsubscribe_all(ctx->ptr, logosdelivery_waku_filter_unsubscribe_all_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryChannelCreateReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryChannelCreateReplyFn fn; void* user_data; } LogosDeliveryChannelCreateCallBox;
static void logosdelivery_channel_create_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryChannelCreateCallBox* box = (LogosDeliveryChannelCreateCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_channel_create(const LogosDeliveryCtx* ctx, NimFfiStr channelIdStr, NimFfiStr contentTopicStr, NimFfiStr senderIdStr, NimFfiStr encryptionStr, LogosDeliveryChannelCreateReplyFn on_reply, void* user_data) {
    LogosdeliveryChannelCreateReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.channelIdStr = channelIdStr;
    ffi_req.contentTopicStr = contentTopicStr;
    ffi_req.senderIdStr = senderIdStr;
    ffi_req.encryptionStr = encryptionStr;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryChannelCreateReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryChannelCreateCallBox* box = (LogosDeliveryChannelCreateCallBox*)malloc(sizeof(LogosDeliveryChannelCreateCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_channel_create(ctx->ptr, logosdelivery_channel_create_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryChannelSendReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryChannelSendReplyFn fn; void* user_data; } LogosDeliveryChannelSendCallBox;
static void logosdelivery_channel_send_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryChannelSendCallBox* box = (LogosDeliveryChannelSendCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_channel_send(const LogosDeliveryCtx* ctx, NimFfiStr channelIdStr, const ChannelSendRequest* req, LogosDeliveryChannelSendReplyFn on_reply, void* user_data) {
    LogosdeliveryChannelSendReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.channelIdStr = channelIdStr;
    ffi_req.req = *req;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryChannelSendReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryChannelSendCallBox* box = (LogosDeliveryChannelSendCallBox*)malloc(sizeof(LogosDeliveryChannelSendCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_channel_send(ctx->ptr, logosdelivery_channel_send_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

typedef void (*LogosDeliveryChannelCloseReplyFn)(int err_code, const NimFfiStr* reply, const char* err_msg, void* user_data);
typedef struct { LogosDeliveryChannelCloseReplyFn fn; void* user_data; } LogosDeliveryChannelCloseCallBox;
static void logosdelivery_channel_close_reply_trampoline(int ret, const char* msg, size_t len, void* ud) {
    LogosDeliveryChannelCloseCallBox* box = (LogosDeliveryChannelCloseCallBox*)ud;
    /* Non-terminal progress ping: keep the box for the terminal reply. */
    if (ret == NIMFFI_RET_STALE_WARN) return;
    if (!box->fn) {
        free(box);
        return;
    }
    if (ret != 0) {
        char* em = nimffi_dup_cstr_n(msg ? msg : "", msg ? len : 0);
        box->fn(ret, NULL, em ? em : "FFI call failed", box->user_data);
        free(em);
        free(box);
        return;
    }
    char* err = NULL;
    NimFfiStr out;
    memset(&out, 0, sizeof(out));
    int dec = nimffi_decode_from_buf(logosdelivery_decv_Str, (const uint8_t*)msg, len, &out, &err);
    if (dec != 0) {
        box->fn(-1, NULL, err ? err : "decode failed", box->user_data);
        free(err);
        nimffi_free_str(&out);
        free(box);
        return;
    }
    box->fn(NIMFFI_RET_OK, &out, NULL, box->user_data);
    nimffi_free_str(&out);
    free(box);
}
static inline int logosdelivery_ctx_channel_close(const LogosDeliveryCtx* ctx, NimFfiStr channelIdStr, LogosDeliveryChannelCloseReplyFn on_reply, void* user_data) {
    LogosdeliveryChannelCloseReq ffi_req;
    memset(&ffi_req, 0, sizeof(ffi_req));
    ffi_req.channelIdStr = channelIdStr;
    uint8_t* req_buf = NULL;
    size_t req_len = 0;
    char* err = NULL;
    if (nimffi_encode_to_buf(logosdelivery_encv_LogosdeliveryChannelCloseReq, &ffi_req, &req_buf, &req_len, &err) != 0) {
        if (on_reply) on_reply(-1, NULL, err ? err : "encode failed", user_data);
        free(err);
        return -1;
    }
    LogosDeliveryChannelCloseCallBox* box = (LogosDeliveryChannelCloseCallBox*)malloc(sizeof(LogosDeliveryChannelCloseCallBox));
    if (!box) {
        free(req_buf);
        if (on_reply) on_reply(-1, NULL, "out of memory", user_data);
        return -1;
    }
    box->fn = on_reply;
    box->user_data = user_data;
    int ret = logosdelivery_channel_close(ctx->ptr, logosdelivery_channel_close_reply_trampoline, box, req_buf, req_len);
    free(req_buf);
    if (ret == NIMFFI_RET_MISSING_CALLBACK) {
        if (on_reply) on_reply(-1, NULL, "RET_MISSING_CALLBACK (internal error)", user_data);
        free(box);
        return -1;
    }
    return 0;
}

#endif /* NIM_FFI_LIB_LOGOSDELIVERY_H_INCLUDED */
