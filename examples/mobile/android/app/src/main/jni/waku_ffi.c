#include "../../../../../../../library/ffi_callback.h"
#include "liblogosdelivery_kernel.h"
#include <android/log.h>
#include <jni.h>
#include <pthread.h>
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

// Result callbacks run on the library FFI thread. JNI methods keep this state
// alive until the terminal callback arrives so a request cannot publish a
// successful result before its real callback (or error) is available.
typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  bool completed;
  cb_result result;
} cb_result_waiter;

// cb_env is a struct passed as userdata when setting up the event callback.
// This is so we can pass the pointer back to kotlin to indicate which instance
// of waku received the message, and also so we can have access to `env` from
// within the event callback
typedef struct {
  jlong wakuPtr;
  JNIEnv *env;
} cb_env;

// Frees the message owned by a callback result.
static void clear_cb_result(cb_result *result) {
  if (result != NULL) {
    if (result->message != NULL) {
      free(result->message);
      result->message = NULL;
    }
  }
}

static char *copy_callback_message(const char *message, size_t length) {
  if (message == NULL && length != 0) {
    return NULL;
  }

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
    cb_result *result, bool error, const char *message, size_t length) {
  result->error = error;
  result->message = copy_callback_message(message, length);
  if (result->message == NULL) {
    result->error = true;
  }
}

static void init_callback_waiter(cb_result_waiter *waiter) {
  waiter->completed = false;
  waiter->result.error = false;
  waiter->result.message = NULL;
  if (pthread_mutex_init(&waiter->mutex, NULL) != 0) {
    abort();
  }
  if (pthread_cond_init(&waiter->condition, NULL) != 0) {
    pthread_mutex_destroy(&waiter->mutex);
    abort();
  }
}

static void complete_callback_result(cb_result_waiter *waiter,
                                     bool error,
                                     const char *message,
                                     size_t length) {
  if (pthread_mutex_lock(&waiter->mutex) != 0) {
    abort();
  }

  if (!waiter->completed) {
    set_callback_result(&waiter->result, error, message, length);
    waiter->completed = true;
    if (pthread_cond_signal(&waiter->condition) != 0) {
      abort();
    }
  }

  if (pthread_mutex_unlock(&waiter->mutex) != 0) {
    abort();
  }
}

static void complete_dispatch_failure(cb_result_waiter *waiter, int status) {
  char message[64];
  int length = snprintf(message, sizeof(message),
                        "request submission failed with status %d", status);
  if (length < 0) {
    complete_callback_result(waiter, true, "request submission failed",
                             sizeof("request submission failed") - 1);
    return;
  }
  if ((size_t)length >= sizeof(message)) {
    length = sizeof(message) - 1;
  }
  complete_callback_result(waiter, true, message, (size_t)length);
}

static cb_result *wait_for_callback_result(cb_result_waiter *waiter) {
  if (pthread_mutex_lock(&waiter->mutex) != 0) {
    abort();
  }

  while (!waiter->completed) {
    if (pthread_cond_wait(&waiter->condition, &waiter->mutex) != 0) {
      abort();
    }
  }

  if (pthread_mutex_unlock(&waiter->mutex) != 0) {
    abort();
  }
  return &waiter->result;
}

static cb_result *submit_request_and_wait(cb_result_waiter *waiter,
                                          int status) {
  if (status != RET_OK) {
    complete_dispatch_failure(waiter, status);
  }
  return wait_for_callback_result(waiter);
}

static void destroy_callback_waiter(cb_result_waiter *waiter) {
  clear_cb_result(&waiter->result);
  if (pthread_cond_destroy(&waiter->condition) != 0) {
    abort();
  }
  if (pthread_mutex_destroy(&waiter->mutex) != 0) {
    abort();
  }
}

static const char *callback_result_message(const cb_result *result) {
  if (result == NULL) {
    return "ok";
  }
  if (result->message != NULL) {
    return result->message;
  }
  if (result->error) {
    return "failed to copy callback result";
  }
  return "";
}

// callback executed by libwaku functions. It expects user_data to be a
// cb_result_waiter*.
void on_response(int ret, const char *msg, size_t len, void *user_data) {
  if (ret == RET_STALE_WARN || user_data == NULL) {
    return;
  }

  cb_result_waiter *waiter = (cb_result_waiter *)user_data;
  if (ret != RET_OK) {
    complete_callback_result(waiter, true, msg, len);
    return;
  }

  if (len == 0) {
    len = 14;
    msg = "on_response-ok";
  } else {
    const char *payload;
    size_t payload_len;
    if (!logosdelivery_decode_cbor_reply(msg, len, &payload, &payload_len)) {
      complete_callback_result(waiter, true, "invalid CBOR request reply",
                               sizeof("invalid CBOR request reply") - 1);
      return;
    }
    msg = payload;
    len = payload_len;
  }

  complete_callback_result(waiter, false, msg, len);
}

static void destroy_failed_node(void *waku_ptr) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  (void)submit_request_and_wait(
      &waiter, logosdelivery_destroy(waku_ptr, on_response, &waiter));
  destroy_callback_waiter(&waiter);
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
    message = (*env)->NewStringUTF(env, callback_result_message(result));
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
  if (result == NULL) {
    error = true;
    message = (*env)->NewStringUTF(env, "node creation did not return a callback result");
    wakuPtr = -1;
  } else if (result->error) {
    error = true;
    message = (*env)->NewStringUTF(env, callback_result_message(result));
    wakuPtr = -1;
  } else if (ptr == NULL) {
    error = true;
    message = (*env)->NewStringUTF(env, "node creation returned no context");
    wakuPtr = -1;
  } else {
    error = false;
    message = (*env)->NewStringUTF(env, callback_result_message(result));
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
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  void *wakuPtr = logosdelivery_create_node(config, on_response, &waiter);
  if (wakuPtr == NULL) {
    complete_dispatch_failure(&waiter, RET_ERR);
  }
  cb_result *result = wait_for_callback_result(&waiter);
  jobject response = to_jni_ptr(env, result, wakuPtr);
  if (result->error && wakuPtr != NULL) {
    destroy_failed_node(wakuPtr);
  }
  (*env)->ReleaseStringUTFChars(env, configJson, config);
  destroy_callback_waiter(&waiter);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuStart(JNIEnv *env, jobject thiz,
                                             jlong wakuPtr) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  cb_result *result = submit_request_and_wait(
      &waiter, logosdelivery_start_node((void *)wakuPtr, on_response, &waiter));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuVersion(JNIEnv *env, jobject thiz,
                                               jlong wakuPtr) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  cb_result *result = submit_request_and_wait(
      &waiter, waku_version((void *)wakuPtr, on_response, &waiter));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuStop(JNIEnv *env, jobject thiz,
                                            jlong wakuPtr) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  cb_result *result = submit_request_and_wait(
      &waiter, logosdelivery_stop_node((void *)wakuPtr, on_response, &waiter));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuDestroy(JNIEnv *env, jobject thiz,
                                               jlong wakuPtr) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  cb_result *result = submit_request_and_wait(
      &waiter, logosdelivery_destroy((void *)wakuPtr, on_response, &waiter));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuConnect(JNIEnv *env, jobject thiz,
                                               jlong wakuPtr,
                                               jstring peerMultiAddr,
                                               jint timeoutMs) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  const char *peer = (*env)->GetStringUTFChars(env, peerMultiAddr, 0);
  cb_result *result = submit_request_and_wait(
      &waiter, waku_connect((void *)wakuPtr, on_response, &waiter, peer,
                            timeoutMs));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  (*env)->ReleaseStringUTFChars(env, peerMultiAddr, peer);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuListenAddresses(JNIEnv *env,
                                                       jobject thiz,
                                                       jlong wakuPtr) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  cb_result *result = submit_request_and_wait(
      &waiter, waku_listen_addresses((void *)wakuPtr, on_response, &waiter));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuRelayPublish(JNIEnv *env, jobject thiz,
                                                    jlong wakuPtr,
                                                    jstring pubsubTopic,
                                                    jstring jsonWakuMessage,
                                                    jint timeoutMs) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  const char *topic = (*env)->GetStringUTFChars(env, pubsubTopic, 0);
  const char *msg = (*env)->GetStringUTFChars(env, jsonWakuMessage, 0);
  cb_result *result = submit_request_and_wait(
      &waiter, waku_relay_publish((void *)wakuPtr, on_response, &waiter, topic,
                                  msg, timeoutMs));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  (*env)->ReleaseStringUTFChars(env, pubsubTopic, topic);
  (*env)->ReleaseStringUTFChars(env, jsonWakuMessage, msg);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuRelaySubscribe(JNIEnv *env, jobject thiz,
                                                      jlong wakuPtr,
                                                      jstring pubsubTopic) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  const char *topic = (*env)->GetStringUTFChars(env, pubsubTopic, 0);
  cb_result *result = submit_request_and_wait(
      &waiter, waku_relay_subscribe((void *)wakuPtr, on_response, &waiter,
                                    topic));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  (*env)->ReleaseStringUTFChars(env, pubsubTopic, topic);
  return response;
}

jobject Java_com_mobile_WakuModule_wakuRelayUnsubscribe(JNIEnv *env,
                                                        jobject thiz,
                                                        jlong wakuPtr,
                                                        jstring pubsubTopic) {
  cb_result_waiter waiter;
  init_callback_waiter(&waiter);
  const char *topic = (*env)->GetStringUTFChars(env, pubsubTopic, 0);
  cb_result *result = submit_request_and_wait(
      &waiter, waku_relay_unsubscribe((void *)wakuPtr, on_response, &waiter,
                                      topic));
  jobject response = to_jni_result(env, result);
  destroy_callback_waiter(&waiter);
  (*env)->ReleaseStringUTFChars(env, pubsubTopic, topic);
  return response;
}

void wk_callback(int callerRet, const char *msg, size_t len, void *userData) {
  cb_env *c = (cb_env *)userData;

  // TODO: might be too much overhead to attach/detach per call?
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
