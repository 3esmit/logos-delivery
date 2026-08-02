use std::ffi::CString;
use std::os::raw::{c_char, c_int, c_void};
use std::{thread, time};

use waku_rust_simple_example::{FFICallback, RequestWaiter};

extern "C" {
    fn logosdelivery_create_node(
        config_json: *const c_char,
        callback: FFICallback,
        user_data: *const c_void,
    ) -> *mut c_void;

    fn waku_version(ctx: *mut c_void, callback: FFICallback, user_data: *const c_void) -> c_int;

    fn logosdelivery_start_node(
        ctx: *mut c_void,
        callback: FFICallback,
        user_data: *const c_void,
    ) -> c_int;

    fn waku_default_pubsub_topic(
        ctx: *mut c_void,
        callback: FFICallback,
        user_data: *const c_void,
    ) -> c_int;
}

const REQUEST_TIMEOUT: time::Duration = time::Duration::from_secs(30);

fn wait_for_request(request: impl FnOnce(FFICallback, *const c_void)) -> Result<Vec<u8>, String> {
    let waiter = RequestWaiter::new();
    request(waiter.callback(), waiter.user_data());
    waiter.wait(REQUEST_TIMEOUT)
}

fn run() -> Result<(), String> {
    let config_json = "\
    { \
        \"mode\": \"Core\",\
        \"messagingOverrides\": { \
            \"listen-address\": \"127.0.0.1\",\
            \"tcp-port\": 60000, \
            \"nodekey\": \"0d714a1fada214dead6dc9c7274581ec20ff292451866e7d6d677dc818e8ccd2\", \
            \"log-level\": \"DEBUG\"\
        }\
    }";
    let config_json = CString::new(config_json)
        .map_err(|_| "node configuration contains an interior NUL byte".to_owned())?;

    let create_waiter = RequestWaiter::new();
    let config_json = create_waiter.retain_c_string(config_json);
    let ctx = unsafe {
        logosdelivery_create_node(
            config_json,
            create_waiter.callback(),
            create_waiter.user_data(),
        )
    };
    create_waiter.wait(REQUEST_TIMEOUT)?;
    if ctx.is_null() {
        return Err("node creation completed without a context".to_owned());
    }

    let version = wait_for_request(|callback, user_data| unsafe {
        let _ = waku_version(ctx, callback, user_data);
    })?;
    println!("Version: {}", String::from_utf8_lossy(&version));

    let default_pubsub_topic = wait_for_request(|callback, user_data| unsafe {
        let _ = waku_default_pubsub_topic(ctx, callback, user_data);
    })?;
    println!(
        "Default pubsubTopic: {}",
        String::from_utf8_lossy(&default_pubsub_topic)
    );

    wait_for_request(|callback, user_data| unsafe {
        let _ = logosdelivery_start_node(ctx, callback, user_data);
    })?;
    println!("Node started");

    loop {
        thread::sleep(time::Duration::from_secs(10));
    }
}

fn main() {
    if let Err(error) = run() {
        eprintln!("logos-delivery example failed: {error}");
        std::process::exit(1);
    }
}
