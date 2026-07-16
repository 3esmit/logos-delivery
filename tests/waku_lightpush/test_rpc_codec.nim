{.used.}

import results, testutils/unittests
import
  logos_delivery/waku/waku_lightpush/rpc,
  logos_delivery/waku/waku_lightpush/rpc_codec,
  logos_delivery/waku/waku_core

# response: requestId "x" (0a 01 78) + statusCode 200 in field 10 (50 c8 01)
const StatusResponse200 = @[byte 0x0a, 0x01, 0x78, 0x50, 0xc8, 0x01]

suite "Waku Lightpush v3 - codec wire format":
  test "LightPushResponse emits statusCode (distinct uint32)":
    let resp = LightPushResponse(
      requestId: "x",
      statusCode: LightPushStatusCode(200),
      statusDesc: Opt.none(string),
      relayPeerCount: Opt.none(uint32),
    )
    check resp.encode() == StatusResponse200

  test "LightPushResponse round-trips with all fields":
    let resp = LightPushResponse(
      requestId: "req-1",
      statusCode: LightPushStatusCode(429),
      statusDesc: Opt.some("too many"),
      relayPeerCount: Opt.some(3'u32),
    )
    let decoded = LightPushResponse.decode(resp.encode())
    check decoded.isOk()
    check decoded.get().requestId == resp.requestId
    check decoded.get().statusCode == resp.statusCode
    check decoded.get().statusDesc == resp.statusDesc
    check decoded.get().relayPeerCount == resp.relayPeerCount

  test "LightpushRequest round-trips with nested WakuMessage":
    let req = LightpushRequest(
      requestId: "req-2",
      pubSubTopic: Opt.some("/waku/2/rs/0/0"),
      message: WakuMessage(payload: @[byte 1, 2, 3], contentTopic: "/a/1/b/c"),
    )
    let decoded = LightpushRequest.decode(req.encode())
    check decoded.isOk()
    check decoded.get().requestId == req.requestId
    check decoded.get().pubSubTopic == req.pubSubTopic
    check decoded.get().message == req.message
