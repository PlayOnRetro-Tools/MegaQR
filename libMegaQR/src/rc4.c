#include "rc4.h"

void rc4_init(rc4_ctx *ctx, const u8 *key, u32 keylen)
{
    u16 j = 0;
    for (u16 i = 0; i < 256; i++) ctx->s[i] = i;
    for (u16 i = 0; i < 256; i++)
    {
        j = (j + ctx->s[i] + key[i % keylen]) & 0xFF;
        u8 tmp = ctx->s[i];
        ctx->s[i] = ctx->s[j];
        ctx->s[j] = tmp;
    }
    ctx->i = ctx->j = 0;
}

void rc4_crypt(rc4_ctx *ctx, u8 *data, u32 len)
{
    for (u32 k = 0; k < len; k++)
    {
        ctx->i = (ctx->i + 1) & 0xFF;
        ctx->j = (ctx->j + ctx->s[ctx->i]) & 0xFF;
        u8 tmp = ctx->s[ctx->i];
        ctx->s[ctx->i] = ctx->s[ctx->j];
        ctx->s[ctx->j] = tmp;
        data[k] ^= ctx->s[(ctx->s[ctx->i] + ctx->s[ctx->j]) & 0xFF];
    }
}
