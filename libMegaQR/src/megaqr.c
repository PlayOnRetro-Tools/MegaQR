#include "megaqr.h"
#include "base64.h"
#include "config.h"
#include "hmac.h"
#include "internal.h"
#include "micro_aes.h"
#include "nonce.h"
#include "qrcode.h"
#include "render.h"
#include "util.h"

#define BUFFER_SIZE 128

extern const char *url;

// Defined in client code
extern const QRGameConfig _game_config;

#if MEGAQR_DEBUG == 1
static void log_frame(QRframe const *const frame)
{
    kprintf("frame size: %ld bytes", sizeof(QRframe));

    char *iv = bytes_to_hex(frame->iv, GAME_KEY_SIZE);
    kprintf("iv: %s", iv);
    free(iv);

    char *hash = bytes_to_hex(frame->hash, SHA1_HASH_SIZE);
    kprintf("sha1: %s", hash);
    free(hash);

    char *cipherhex = bytes_to_hex(frame->ciphertext, sizeof(QRPayload));
    kprintf("ciphertext: %s", cipherhex);
    free(cipherhex);
}
#endif

void megaqr_generate(ScoreEntry const *const entry, const u16 x, const u16 y, const u16 index, const u8 pal)
{
    QRframe frame;
    frame.version = _MEGA_QR_VERSION_;
    frame.gameID = _game_config.gameID;

    // Random number for replay submission detection
    QRPayload payload;
    payload.nonce = xorshift32();

    memcpy(&payload.entry, entry, sizeof(ScoreEntry));

    u8 game_key_bytes[GAME_KEY_SIZE];
#if MEGAQR_DEBUG == 1
    hex2bytes("606550e2c432dbd57ea9e9494cb73d6a", game_key_bytes);
#else
    hex2bytes(_game_config.key, game_key_bytes);
#endif

    // Encrypt payload
    nonce_generate(frame.iv, GAME_KEY_SIZE);
    AES_CTR_encrypt(game_key_bytes, frame.iv, &payload, sizeof(QRPayload), frame.ciphertext);

    // Signature
    hmac_sha1(game_key_bytes, GAME_KEY_SIZE, (u8 *)frame.ciphertext, sizeof(QRPayload), frame.hash);

    // Base64 encode qrframe
    char b64String[BUFFER_SIZE];
    bintob64(b64String, (u8 *)&frame, sizeof(QRframe));

    // Generate request
    char request[BUFFER_SIZE];
    concat_url_b64(request, BUFFER_SIZE, url, b64String);

    // Generate the QRCode
    u8 qrcode[BUFFER_SIZE * 2];
    u8 tmpbuffer[BUFFER_SIZE * 2];
    QRCode_EncodeText(request, tmpbuffer, qrcode, QRCODE_ECC_LOW, QRCODE_MASK_7, FALSE);

    render_qr(qrcode, QRCode_GetSize(qrcode), index, pal, x, y);

#if MEGAQR_DEBUG == 1
    kprintf("request size: %d bytes", strlen(request));
    kprintf("megaqr frame version: %d", _MEGA_QR_VERSION_);
    kprintf("qrcode mask level: %d", QRCODE_MASK_7);
    log_frame(&frame);
#endif
}
