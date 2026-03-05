#pragma once

#include <genesis.h>

// RC4 stream cypher

typedef struct
{
    u8 s[256];
    u8 i, j;
} rc4_ctx;

void rc4_init(rc4_ctx *const ctx, const u8 *key, const u32 keylen);
void rc4_crypt(rc4_ctx *const ctx, u8 *const data, const u32 len);
