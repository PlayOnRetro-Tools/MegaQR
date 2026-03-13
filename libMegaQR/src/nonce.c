#include "nonce.h"

static u32 xorshift_state;

static void xorshift_seed(u32 seed)
{
    xorshift_state = seed ? seed : 0xDEADBEEF; // Never allow zero state
}

u32 xorshift32(void)
{
    xorshift_seed(random());

    u32 x = xorshift_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    xorshift_state = x;

    return x;
}

void nonce_generate(u8 *const nonce, const u32 len)
{
    for (u16 i = 0; i < len; i += 4)
    {
        const u32 r = xorshift32();

        nonce[i + 0] = (r >> 0) & 0xFF;
        nonce[i + 1] = (r >> 8) & 0xFF;
        nonce[i + 2] = (r >> 16) & 0xFF;
        nonce[i + 3] = (r >> 24) & 0xFF;
    }
}
