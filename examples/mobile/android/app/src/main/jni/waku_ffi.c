#include "../../../../../../../library/ffi_callback.h"
#include "liblogosdelivery_kernel.h"
#include <android/log.h>
#include <jni.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define APPNAME "waku-jni"
#define LOGD(TAG) __android_log_print(ANDROID_LOG_DEBUG , APPNAME,TAG);

// cb_result represents a response received when executing a callback.
// If `error` is true, `message` will contain the error message description
// otherwise, it will contain the result of the callback execution
typedef struct {
  bool error;
  char *message;
} cb_result;

// cb_env is a struct passed as userdata when setting up the event callback.
// This is so we can pass the pointer back to kotlin to indicate which instance
// of waku received the message, and also so we can have access to `env` from
// within the event callback
typedef struct {
  jlong wakuPtr;
  JNIEnv *env;
} cb_env;

// frees the results associated to the allocation of a cb_result
void free_cb_result(cb_result *result) {
  if (result != NULL) {
    if (result->message != NULL) {
      free(result->message);
      result->message = NULL;
    }
    free(result);
    result = NULL;
  }
}

static char *copy_callback_message(const char *message, size_t length) {
  char *copy = malloc(length + 1);
  if (copy == NULL) {
    return NULL;
  }
  if (message != NULL && length != 0) {
    memcpy(copy, message, length);
  }
  copy[length] = '\0';
  return copy;
}

static void set_callback_result(
    cb_result **data_ref, bool error, const char *message, size_t length) {
  *data_ref = malloc(sizeof(cb_result));
  (*data_ref)->error = error;
  (*data_ref)->message = copy_callback_message(message, length);
}

// callback executed by libwaku functions. It expects user_data to be a
// cb_result*.
void on_response(int ret, const char *msg, size_t len, void *user_data) {
  if (ret == RET_STALE_WARN || user_data == NULL) {
    return;
  }

  cb_result **data_ref = (cb_result **)user_data;
  if (ret != RET_OK) {
    set_callback_result(data_ref, true, msg, len);
    return;
  }

  if (len == 0) {
    len = 14;
    msg = "on_response-ok";
  } else {
    const char *payload;
    size_t payload_len;
    if (!logosdelivery_decode_cbor_reply(msg, len, &payload, &payload_len)) {
      set_callback_result(data_ref, true, "invalid CBOR request reply",
                          sizeof("invalid CBOR request reply") - 1);
      return;
    }
    msg = payload;
    len = payload_len;
  }

  set_callback_result(data_ref, false, msg, len);
}

// converts a cb_result into an instance of the kotlin WakuResult class
jobject to_jni_result(JNIEnv *env, cb_result *result) {
  jclass myStructClass = (*env)->FindClass(env, "com/mobile/WakuResult");
  jmethodID constructor = (*env)->GetMethodID(env, myStructClass, "<init>",
                                              "(ZLjava/lang/String;)V");

  jboolean error;
  jstring message;
  if (result != NULL) {
    error = result->error;
    message = (*env)->NewStringUTF(env, result->message);
  } else {
    error = false;
    message = (*env)->NewStringUTF(env, "ok");
  }

  jobject response =
      (*env)->NewObject(env, myStructClass, constructor, error, message);

  // Free the intermediate message var
  (*env)->DeleteLocalRef(env, message);

  return response;
}

// converts a cb_result into an instance of the kotlin WakuPtr class
jobject to_jni_ptr(JNIEnv *env, cb_result *result, void *ptr) {
  jclass myStructClass = (*env)->FindClass(env, "com/mobile/WakuPtr");
  jmethodID constructor = (*env)->GetMethodID(env, myStructClass, "<init>",
                                              "(ZLjava/lang/String;J)V");

  jboolean error;
  jstring message;
  jlong wakuPtr;
  if (result != NULL) {
    error = result->error;
    message = (*env)->NewStringUTF(env, result->message);
    wakuPtr = -1;
  } else {
    error = false;
    message = (*env)->NewStringUTF(env, "ok");
    wakuPtr = (jlong)ptr;
  }

  jobject response = (*env)->NewObject(env, myStructClass, constructor, error,
                                       message, wakuPtr);

  // Free the intermediate message var
  (*env)->DeleteLocalRef(env, message);

  return response;
}

// libwaku functions
// ============================================================================

// JVM, required for executing JNI functions in a third party thread.
JavaVM *jvm;
static jobject jClassLoader;
static jmethodID jLoadClass;

JNIEnv *getEnv() {
  JNIEnv *env;
  int status = (*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_6);
  if (status < 0) {
    status = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
    assert(status == JNI_OK && "could not obtain env");
  }
  return env;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *pjvm, void *reserved) {
  jvm = pjvm; // cache the JavaVM pointer
  JNIEnv *env = getEnv();

  jclass jLibraryClass =
      (*env)->FindClass(env, "com/mobile/EventCallbackManager");
  jclass jClassRef = (*env)->GetObjectClass(env, jLibraryClass);
  jclass jClassLoaderClass = (*env)->FindClass(env, "java/lang/ClassLoader");
  jmethodID getClassLoader = (*env)->GetMethodID(
      env, jClassRef, "getClassLoader", "()Ljava/lang/ClassLoader;");

  jobject jClassLoaderLocal =
      (*env)->CallObjectMethod(env, jLibraryClass, getClassLoader);
  jLoadClass = (*env)->GetMethodID(env, jClassLoaderClass, "loadClass",
                                   "(Ljava/lang/String;)Ljava/lang/Class;");
  jClassLoader = (*env)->NewGlobalRef(env, jClassLoaderLocal);

  (*env)->DeleteLocalRef(env, jClassLoaderLocal);
  (*env)->DeleteLocalRef(env, jClassLoaderClass);
  (*env)->DeleteLocalRef(env, jClassRef);
  (*env)->DeleteLocalRef(env, jLibraryClass);

  return JNI_VERSION_1_6;
}

jclass loadClass(JNIEnv *env, const char *className) {
  jstring jName = (*env)->NewStringUTF(env, className);
  jclass jClass = (*env)->CallObjectMethod(env, jClassLoader, jLoadClass, jName);
  assert((*env)->ExceptionCheck(env) == JNI_FALSE && "class could not be loaded");
  (*env)->DeleteLocalRef(env, jName);
  return jClass;
}

void Java_com_mobile_WakuModule_wakuSetup(JNIEnv *env, jobject thiz) {
  LOGD("log example for debugging purposes...")
}

jobject Java_com_mobile_WakuModule_wakuNew(JNIEnv *env, jobject thiz,
                                           jstring configJson) {
  const char *config = (*env)->GetStringUTFChars(env, configJson, 0);
  cb_result *result = NULL;
  void *wakuPtr = logosdelivery_create_node(config, on_response, (void *)&result);
  jobject response = to_jni_ptr(env, result, wakuPtr);
  (*env)->ReleaseStringUTFChars(env, configJson, config);
  free_cb_result(result);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuStart(JNIEnv *env, jobject thiz,
                                             jlong wakuPtr) {
  cb_result *result = NULL;
  logosdelivery_start_node((void *)wakuPtr, on_response, &result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuVersion(JNIEnv *env, jobject thiz,
                                               jlong wakuPtr) {
  cb_result *result = NULL;
  waku_version((void *)wakuPtr, on_response, (void *)&result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuStop(JNIEnv *env, jobject thiz,
                                            jlong wakuPtr) {
  cb_result *result = NULL;
  logosdelivery_stop_node((void *)wakuPtr, on_response, &result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuDestroy(JNIEnv *env, jobject thiz,
                                               jlong wakuPtr) {
  cb_result *result = NULL;
  logosdelivery_destroy((void *)wakuPtr, on_response, &result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuConnect(JNIEnv *env, jobject thiz,
                                               jlong wakuPtr,
                                               jstring peerMultiAddr,
                                               jint timeoutMs) {
  cb_result *result = NULL;
  const char *peer = (*env)->GetStringUTFChars(env, peerMultiAddr, 0);
  waku_connect((void *)wakuPtr, peer, timeoutMs, on_response, &result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  (*env)->ReleaseStringUTFChars(env, peerMultiAddr, peer);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuListenAddresses(JNIEnv *env,
                                                       jobject thiz,
                                                       jlong wakuPtr) {
  cb_result *result = NULL;
  waku_listen_addresses((void *)wakuPtr, on_response, (void *)&result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuRelayPublish(JNIEnv *env, jobject thiz,
                                                    jlong wakuPtr,
                                                    jstring pubsubTopic,
                                                    jstring jsonWakuMessage,
                                                    jint timeoutMs) {
  cb_result *result = NULL;
  const char *topic = (*env)->GetStringUTFChars(env, pubsubTopic, 0);
  const char *msg = (*env)->GetStringUTFChars(env, jsonWakuMessage, 0);
  waku_relay_publish((void *)wakuPtr, topic, msg, timeoutMs, on_response,
                     (void *)&result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  (*env)->ReleaseStringUTFChars(env, pubsubTopic, topic);
  (*env)->ReleaseStringUTFChars(env, jsonWakuMessage, msg);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuRelaySubscribe(JNIEnv *env, jobject thiz,
                                                      jlong wakuPtr,
                                                      jstring pubsubTopic) {
  cb_result *result = NULL;
  const char *topic = (*env)->GetStringUTFChars(env, pubsubTopic, 0);
  waku_relay_subscribe((void *)wakuPtr, topic, on_response, (void *)&result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  (*env)->ReleaseStringUTFChars(env, pubsubTopic, topic);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuRelayUnsubscribe(JNIEnv *env,
                                                        jobject thiz,
                                                        jlong wakuPtr,
                                                        jstring pubsubTopic) {
  cb_result *result = NULL;
  const char *topic = (*env)->GetStringUTFChars(env, pubsubTopic, 0);
  waku_relay_unsubscribe((void *)wakuPtr, topic, on_response, (void *)&result);
  jobject response = to_jni_result(env, result);
  free_cb_result(result);
  (*env)->ReleaseStringUTFChars(env, pubsubTopic, topic);
  return response;
}

void wk_callback(int callerRet, const char *msg, size_t len, void *userData) {
  cb_env *c = (cb_env *)userData;

  // TODO: might be too much overhead to attach/detach per call?
  JNIEnv *env = c->env;
  JNIEnv *attachedEnv = NULL;
  assert((*jvm)->AttachCurrentThread(jvm, &attachedEnv, NULL) == JNI_OK && "could not attach to current thread");

  jclass clazz = loadClass(attachedEnv, "com/mobile/EventCallbackManager");

  jmethodID methodID =
      (*attachedEnv)
          ->GetStaticMethodID(attachedEnv, clazz, "execEventCallback", "(JLjava/lang/String;)V");

  jstring message = (*attachedEnv)->NewStringUTF(attachedEnv, msg);
  (*attachedEnv)->CallStaticVoidMethod(attachedEnv, clazz, methodID, c->wakuPtr, message);

  (*attachedEnv)->DeleteLocalRef(attachedEnv, clazz);

  (*attachedEnv)->DeleteLocalRef(attachedEnv, message);

  (*jvm)->DetachCurrentThread(jvm);
}

void Java_com_mobile_WakuModule_wakuSetEventCallback(JNIEnv *env, jobject thiz,
                                                     jlong wakuPtr) {
  cb_env *c = (cb_env *)malloc(sizeof(cb_env));
  c->wakuPtr = wakuPtr;
  c->env = env;
  static const char *kEventNames[] = {
      "onMessageSent",            "onMessageError",
      "onMessagePropagated",      "onMessageReceived",
      "onConnectionStatusChange", "onTopicHealthChange",
      "onConnectionChange",       "onReceivedMessage",
      "onChannelMessageReceived", "onChannelMessageSent",
      "onChannelMessageError"};
  for (size_t i = 0; i < sizeof(kEventNames) / sizeof(kEventNames[0]); i++)
    logosdelivery_add_event_listener((void *)wakuPtr, kEventNames[i], wk_callback,
                                     (void *)c);
}
