#include "base64.h"
#include "config.h"
#include "megaqr.h"
#include "micro_aes.h"
#include "nonce.h"
#include "qrcode.h"
#include "render.h"

static const char *url = "http://192.168.1.38:5000/submit/%s";

// Defined in client code
extern const QRGameConfig _game_config;

static void hex2bytes(const char *hex, u8 *bytes)
{
    u32 shl = 0;
    for (--bytes; *hex; ++hex)
    {
        if (*hex < '0' || 'f' < *hex) continue;
        if ((shl ^= 4) != 0) *++bytes = 0;
        *bytes |= (*hex % 16 + (*hex > '9') * 9) << shl;
    }
}

void qr_generate(ScoreEntry const *const entry, const u16 x, const u16 y, const u16 index, const u8 pal)
{
    ScorePayload payload;
    payload.nonce = xorshift32();
    memcpy(&payload, entry, sizeof(ScoreEntry));

    // Prepare Data Frame
    QRframe frame;
    frame.gameID = _game_config.gameID;

    // Generate random token for AES encryption
    nonce_generate(frame.iv, NONCE_SIZE_BYTES);

    // Prepare game private key
    u8 game_key_bytes[GAME_KEY_SIZE_BYTES];
    hex2bytes(_game_config.key, game_key_bytes);

    // Encrypt payload
    AES_CTR_encrypt(game_key_bytes, frame.iv, &payload, (u32)sizeof(ScorePayload), frame.payload);

    // Encode encrypted data as base64 string
    char b64String[B64_STR_SIZE];
    char text[TEXT_BUFFER_SIZE];
    bintob64(b64String, (u8 *)&frame, sizeof(QRframe));
    sprintf(text, url, b64String);

    // Generate the QRCode
    u8 QR[QRCODE_BUFFER_SIZE];
    u8 buffer[QRCODE_BUFFER_SIZE];
    QRCode_EncodeText(text, buffer, QR, QRCODE_ECC_LOW, QRCODE_MASK_4, FALSE);

    render_qr(QR, QRCode_GetSize(QR), index, pal, x, y);
}
