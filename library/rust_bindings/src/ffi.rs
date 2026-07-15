use std::os::raw::{c_char, c_int, c_void};

pub type FFICallback = unsafe extern "C" fn(
    ret: c_int,
    msg: *const c_char,
    len: usize,
    user_data: *mut c_void,
);

#[link(name = "logosdelivery")]
extern "C" {
    pub fn logosdelivery_create_node(req_cbor: *const u8, req_cbor_len: usize, callback: FFICallback, user_data: *mut c_void) -> *mut c_void;
    pub fn logosdelivery_start_node(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_stop_node(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_destroy(ctx: *mut c_void) -> c_int;
    pub fn logosdelivery_subscribe(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_unsubscribe(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_send(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_get_available_node_info_ids(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_get_node_info(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_get_available_configs(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_peerids_from_peerstore(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_connect(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_disconnect_peer_by_id(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_disconnect_all_peers(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_dial_peer(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_dial_peer_by_id(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_connected_peers_info(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_connected_peers(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_peerids_by_protocol(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_discv5_update_bootnodes(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_dns_discovery(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_start_discv5(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_stop_discv5(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_peer_exchange_request(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_version(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_listen_addresses(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_my_enr(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_my_peerid(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_get_metrics(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_is_online(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_ping_peer(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_get_peers_in_mesh(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_get_num_peers_in_mesh(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_get_connected_peers(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_get_num_connected_peers(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_add_protected_shard(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_subscribe(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_unsubscribe(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_relay_publish(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_default_pubsub_topic(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_content_topic(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_pubsub_topic(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_store_query(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_lightpush_publish(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_filter_subscribe(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_filter_unsubscribe(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn waku_filter_unsubscribe_all(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_channel_create(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_channel_send(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_channel_close(ctx: *mut c_void, callback: FFICallback, user_data: *mut c_void, req_cbor: *const u8, req_cbor_len: usize) -> c_int;
    pub fn logosdelivery_add_event_listener(ctx: *mut c_void, event_name: *const c_char, callback: FFICallback, user_data: *mut c_void) -> u64;
    pub fn logosdelivery_remove_event_listener(ctx: *mut c_void, listener_id: u64) -> c_int;
}
