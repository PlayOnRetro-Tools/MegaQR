#pragma once

#include <genesis.h>

// RC4 stream cypher

typedef struct
{
    u8 s[256];
    u8 i, j;
} rc4_ctx;

void rc4_init(rc4_ctx *ctx, const u8 *key, const u32 keylen);
void rc4_crypt(rc4_ctx *ctx, u8 *data, const u32 len);
