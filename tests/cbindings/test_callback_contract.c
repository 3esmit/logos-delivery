#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../library/ffi_callback.h"

static int terminal_callbacks;

static void record_terminal_callback(int status)
{
  if (status == RET_STALE_WARN)
  {
    return;
  }

  terminal_callbacks++;
}

static void check_decoded_payload(const unsigned char *encoded,
                                  size_t encoded_len,
                                  const unsigned char *expected,
                                  size_t expected_len)
{
  const char *payload = NULL;
  size_t payload_len = 0;

  assert(logosdelivery_decode_cbor_reply(
      (const char *)encoded, encoded_len, &payload, &payload_len));
  assert(payload_len == expected_len);
  assert(expected_len == 0 || memcmp(payload, expected, expected_len) == 0);
}

int main(void)
{
  const unsigned char short_text[] = {0x63, 'o', 'k', '!'};
  const unsigned char short_bytes[] = {0x43, 0x00, 0x01, 0x02};
  const unsigned char empty_text[] = {0x60};
  const unsigned char length_64_text[] = {
      0x78, 0x18,
      'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
      'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
      'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
  const unsigned char length_64_header[] = {0x7b, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x01, 'x'};
  unsigned char length_256_text[3 + 256];
  unsigned char *length_65536_text;
  const char *payload = NULL;
  size_t payload_len = 0;

  check_decoded_payload(short_text, sizeof(short_text), short_text + 1, 3);
  check_decoded_payload(short_bytes, sizeof(short_bytes), short_bytes + 1, 3);
  check_decoded_payload(empty_text, sizeof(empty_text), empty_text + 1, 0);
  check_decoded_payload(length_64_text,
                        sizeof(length_64_text),
                        length_64_text + 2,
                        24);
  check_decoded_payload(length_64_header,
                        sizeof(length_64_header),
                        length_64_header + 9,
                        1);

  length_256_text[0] = 0x79;
  length_256_text[1] = 0x01;
  length_256_text[2] = 0x00;
  memset(length_256_text + 3, 'b', 256);
  check_decoded_payload(length_256_text,
                        sizeof(length_256_text),
                        length_256_text + 3,
                        256);

  length_65536_text = malloc(5 + 65536);
  assert(length_65536_text != NULL);
  length_65536_text[0] = 0x7a;
  length_65536_text[1] = 0x00;
  length_65536_text[2] = 0x01;
  length_65536_text[3] = 0x00;
  length_65536_text[4] = 0x00;
  memset(length_65536_text + 5, 'c', 65536);
  check_decoded_payload(length_65536_text,
                        5 + 65536,
                        length_65536_text + 5,
                        65536);
  free(length_65536_text);

  assert(logosdelivery_decode_cbor_reply(NULL, 0, &payload, &payload_len));
  assert(payload == NULL);
  assert(payload_len == 0);
  assert(!logosdelivery_decode_cbor_reply("\x7f", 1, &payload, &payload_len));
  assert(!logosdelivery_decode_cbor_reply("\x63x", 2, &payload, &payload_len));
  assert(!logosdelivery_decode_cbor_reply("\x63xyz!", 5, &payload, &payload_len));

  record_terminal_callback(RET_STALE_WARN);
  assert(terminal_callbacks == 0);
  record_terminal_callback(RET_OK);
  assert(terminal_callbacks == 1);
  record_terminal_callback(RET_ERR);
  assert(terminal_callbacks == 2);

  return 0;
}
