#include <QObject>
#include <QDebug>
#include <QString>
#include <cstdio>
#include <cstdlib>

#include "../../library/liblogosdelivery_kernel.h"
#include "../../library/ffi_callback.h"

static void print_callback_message(
    const char* prefix, const char* msg, size_t len) {
    std::fputs(prefix, stdout);
    if (msg != nullptr && len != 0) {
        std::fwrite(msg, 1, len, stdout);
    }
    std::fputc('\n', stdout);
}

static void print_request_result(
    int callerRet, const char* msg, size_t len, void* userData) {
    const char* payload = nullptr;
    size_t payload_len = 0;

    (void)userData;
    if (callerRet == RET_STALE_WARN) {
        return;
    }
    if (callerRet == RET_ERR) {
        print_callback_message("Error: ", msg, len);
        return;
    }
    if (callerRet != RET_OK) {
        return;
    }
    if (!logosdelivery_decode_cbor_reply(msg, len, &payload, &payload_len)) {
        std::fputs("Invalid CBOR request reply\n", stderr);
        return;
    }
    if (payload_len != 0) {
        print_callback_message("Request reply: ", payload, payload_len);
    }
}

class WakuHandler : public QObject {
    Q_OBJECT
private:
    static void event_handler(int callerRet, const char* msg, size_t len, void* userData) {
        print_request_result(callerRet, msg, len, userData);
    }

    static void on_event_received(int callerRet, const char* msg, size_t len, void* userData) {
        (void)userData;
        if (callerRet == RET_ERR) {
            print_callback_message("Error: ", msg, len);
            exit(1);
        }
        else if (callerRet == RET_OK) {
            print_callback_message("Receiving event: ", msg, len);
        }
    }

public:
    WakuHandler() : QObject(), ctx(nullptr) {}

    void initialize(const QString& jsonConfig, FFICallBack event_handler, void* userData) {
        ctx = logosdelivery_create_node(jsonConfig.toUtf8().constData(), event_handler, userData);

        for (const char *eventName :
             {"onMessageSent", "onMessageError", "onMessagePropagated",
              "onMessageReceived", "onConnectionStatusChange", "onTopicHealthChange",
              "onConnectionChange", "onReceivedMessage", "onChannelMessageReceived",
              "onChannelMessageSent", "onChannelMessageError"}) {
            logosdelivery_add_event_listener(ctx, eventName, on_event_received, userData);
        }
        qDebug() << "Waku context initialized, ready to start.";
    }

    Q_INVOKABLE void start() {
        if (ctx) {
            logosdelivery_start_node(ctx, event_handler, nullptr);
            qDebug() << "Waku start called with event_handler and userData.";
        } else {
            qDebug() << "Context is not initialized in start.";
        }
    }

    Q_INVOKABLE void stop() {
        if (ctx) {
            logosdelivery_stop_node(ctx, event_handler, nullptr);
            qDebug() << "Waku stop called with event_handler and userData.";
        } else {
            qDebug() << "Context is not initialized in stop.";
        }
    }

    virtual ~WakuHandler() {}

private:
    void* ctx;
};
