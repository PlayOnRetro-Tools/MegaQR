#include "util.h"

char *bytes_to_hex(const uint8_t *data, const size_t len)
{
    char *buf = malloc(len * 2 + 1);
    if (!buf) return NULL;

    static const char hex[] = "0123456789abcdef";

    for (size_t i = 0; i < len; i++)
    {
        buf[i * 2] = hex[data[i] >> 4];
        buf[i * 2 + 1] = hex[data[i] & 0x0F];
    }
    buf[len * 2] = '\0';

    return buf;
}

void hex2bytes(const char *hex, u8 *bytes)
{
    u32 shl = 0;
    for (--bytes; *hex; ++hex)
    {
        if (*hex < '0' || 'f' < *hex) continue;
        if ((shl ^= 4) != 0) *++bytes = 0;
        *bytes |= (*hex % 16 + (*hex > '9') * 9) << shl;
    }
}

void concat_url_b64(char *buf, u32 buf_size, const char *url, const char *b64)
{
    size_t url_len = strlen(url);
    size_t b64_len = strlen(b64);

    if (url_len + b64_len + 1 > buf_size)
        return;

    memcpy(buf, (u8 *)url, url_len);
    memcpy(buf + url_len, (u8 *)b64, b64_len);
    buf[url_len + b64_len] = '\0';
}

char *util_strchr(const char *s, int c)
{
    unsigned char ch = (unsigned char)c;

    while (*s != '\0') {
        if ((unsigned char)*s == ch)
            return (char *)s;
        s++;
    }

    /* Check if searching for the null terminator itself */
    if (ch == '\0')
        return (char *)s;

    return NULL;
}
