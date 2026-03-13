#pragma once

#include <genesis.h>

void nonce_generate(u8 * const nonce, const u32 len);
u32  xorshift32(void);
