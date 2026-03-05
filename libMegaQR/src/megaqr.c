#include <genesis.h>

#include "base64.h"
#include "megaqr.h"
#include "qr.h"
#include "rc4.h"

#define SECRET_KEY     "secretcodeporkey"
#define SECRETKEY_SIZE 16

static const char *url = "http://192.168.1.38:5000/submit/<%s>";

void qr_generate(ScoreEntry const *const entry, const u16 tilex, const u16 tiley, const u16 index)
{
    SYS_disableInts();

    // Don't overwrite original entry data
    ScoreEntry cypher;
    memcpy(&cypher, entry, sizeof(ScoreEntry));

    rc4_ctx ctx;
    rc4_init(&ctx, (u8 *)SECRET_KEY, SECRETKEY_SIZE);
    rc4_crypt(&ctx, (u8 *)&cypher, sizeof(ScoreEntry));

    // Encode encrypted data as base64
    char base64String[32];
    bintob64(base64String, (u8 *)&cypher, sizeof(ScoreEntry));

    char buffer[128];
    sprintf(buffer, url, base64String);
    QR_drawQrCode(buffer, tilex, tiley, index);

    SYS_enableInts();
    SYS_doVBlankProcess();
}
