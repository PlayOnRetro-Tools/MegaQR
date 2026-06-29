#pragma once

#include <genesis.h>

char *bytes_to_hex(const uint8_t *data, const size_t len);
void  hex2bytes(const char *hex, u8 *bytes);
void  concat_url_b64(char *buf, u32 buf_size, const char *url, const char *b64);
char *util_strchr(const char *s, int c);
