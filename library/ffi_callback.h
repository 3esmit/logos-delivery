// Helpers for consuming liblogosdelivery request callbacks safely.
#pragma once
#ifndef LOGOSDELIVERY_FFI_CALLBACK_H
#define LOGOSDELIVERY_FFI_CALLBACK_H

#include "liblogosdelivery.h"

// Decodes one definite-length CBOR text or byte string without allocating.
// Returns nonzero on success and exposes a slice of the input buffer through
// `payload`/`payload_len`. Empty raw RET_OK replies (for example destroy) are
// accepted. The callback buffer remains owned by the library and is valid only
// for the duration of the callback, so callers that need it later must copy it.
static inline int logosdelivery_decode_cbor_reply(
    const char *msg,
    size_t len,
    const char **payload,
    size_t *payload_len) {
  const unsigned char *bytes;
  size_t header_len = 1;
  size_t decoded_len;
  unsigned char additional;

  if (payload == NULL || payload_len == NULL || (msg == NULL && len != 0)) {
    return 0;
  }

  if (len == 0) {
    *payload = msg;
    *payload_len = 0;
    return 1;
  }

  bytes = (const unsigned char *)msg;
  if ((bytes[0] >> 5) != 2 && (bytes[0] >> 5) != 3) {
    return 0;
  }

  additional = bytes[0] & 0x1f;
  if (additional < 24) {
    decoded_len = additional;
  } else if (additional == 24) {
    if (len < 2) {
      return 0;
    }
    header_len = 2;
    decoded_len = bytes[1];
  } else if (additional == 25) {
    if (len < 3) {
      return 0;
    }
    header_len = 3;
    decoded_len = ((size_t)bytes[1] << 8) | (size_t)bytes[2];
  } else if (additional == 26) {
    if (len < 5) {
      return 0;
    }
    header_len = 5;
    decoded_len = ((size_t)bytes[1] << 24) | ((size_t)bytes[2] << 16) |
                  ((size_t)bytes[3] << 8) | (size_t)bytes[4];
  } else if (additional == 27) {
    unsigned long long value;

    if (len < 9) {
      return 0;
    }
    header_len = 9;
    value = ((unsigned long long)bytes[1] << 56) |
            ((unsigned long long)bytes[2] << 48) |
            ((unsigned long long)bytes[3] << 40) |
            ((unsigned long long)bytes[4] << 32) |
            ((unsigned long long)bytes[5] << 24) |
            ((unsigned long long)bytes[6] << 16) |
            ((unsigned long long)bytes[7] << 8) |
            (unsigned long long)bytes[8];
    if (value > (unsigned long long)(size_t)-1) {
      return 0;
    }
    decoded_len = (size_t)value;
  } else {
    return 0;
  }

  if (decoded_len != len - header_len) {
    return 0;
  }

  *payload = msg + header_len;
  *payload_len = decoded_len;
  return 1;
}

#endif /* LOGOSDELIVERY_FFI_CALLBACK_H */
