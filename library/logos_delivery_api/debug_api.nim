import std/[json, strutils]
import logos_delivery/waku/factory/waku_state_info
import tools/confutils/[cli_args, config_option_meta]

proc logosdeliveryGetAvailableNodeInfoIds*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  ## Returns the list of all available node info item ids that
  ## can be queried with `get_node_info_item`.
  return ok($lib.waku.stateInfo.getAllPossibleInfoItemIds())

proc logosdeliveryGetNodeInfo*(
    lib: LogosDelivery, nodeInfoId: string
): Future[Result[string, string]] {.ffi.} =
  ## Returns the content of the node info item with the given id if it exists.
  let infoItemIdEnum =
    try:
      parseEnum[NodeInfoId](nodeInfoId)
    except ValueError:
      return err("Invalid node info id: " & nodeInfoId)

  return ok(lib.waku.stateInfo.getNodeInfoItem(infoItemIdEnum))

proc logosdeliveryGetAvailableConfigs*(
    lib: LogosDelivery
): Future[Result[string, string]] {.ffi.} =
  ## Returns information about the accepted config items.
  let optionMetas: seq[ConfigOptionMeta] = extractConfigOptionMeta(WakuNodeConf)
  var configOptionDetails = newJArray()

  # for confField, confValue in fieldPairs(conf):
  #   defaultConfig[confField] = $confValue

  for meta in optionMetas:
    configOptionDetails.add(
      %*{
        meta.fieldName: meta.typeName & "(" & meta.defaultValue & ")", "desc": meta.desc
      }
    )

  var jsonNode = newJObject()
  jsonNode["configOptions"] = configOptionDetails
  let asString = pretty(jsonNode)
  return ok(pretty(jsonNode))
