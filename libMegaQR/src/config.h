#pragma once

#include "megaqr.h"
#include <genesis.h>

typedef struct __attribute__((packed))
{
    u32        nonce;
    ScoreEntry entry;
} ScorePayload;

typedef struct __attribute__((packed))
{
    u8 gameID;
    u8 iv[16];
    u8 payload[sizeof(ScorePayload)];
} QRframe;

#define GAME_KEY_SIZE_BYTES 16
#define NONCE_SIZE_BYTES    16
#define B64_STR_SIZE        128
#define TEXT_BUFFER_SIZE    128
#define QRCODE_BUFFER_SIZE  256
