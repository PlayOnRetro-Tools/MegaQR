#pragma once

#include "megaqr.h"
#include <genesis.h>

#define GAME_KEY_SIZE  16
#define SHA1_HASH_SIZE 20

typedef struct __attribute__((packed))
{
    u32        nonce;
    ScoreEntry entry;
} QRPayload;

typedef struct __attribute__((packed))
{
    u8  version;
    u16 gameID;
    u8  iv[GAME_KEY_SIZE];
    u8  hash[SHA1_HASH_SIZE];       // Payload hmac
    u8  ciphertext[sizeof(QRPayload)];
} QRframe;

// Current QRFrame version
#define _MEGA_QR_VERSION_ 1
