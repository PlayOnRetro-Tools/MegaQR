#include "base64.h"
#include "config.h"
#include "micro_aes.h"
#include "megaqr.h"
#include "nonce.h"
#include "qrcode.h"
#include "render.h"

static const char *url = "http://192.168.1.37:5000/api/t/";

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

static char *bytes_to_hex(const uint8_t *data, size_t len)
{
    char *buf = malloc(len * 2 + 1);
    if (!buf) return NULL;

    static const char hex[] = "0123456789abcdef";

    for (size_t i = 0; i < len; i++) {
        buf[i * 2]     = hex[data[i] >> 4];
        buf[i * 2 + 1] = hex[data[i] & 0x0F];
    }
    buf[len * 2] = '\0';

    return buf;
}

static void concat_url_b64(char *buf, u32 buf_size, const char *url, const char *b64)
{
    size_t url_len = strlen(url);
    size_t b64_len = strlen(b64);

    if (url_len + b64_len + 1 > buf_size)
        return;

    memcpy(buf, (u8*)url, url_len);
    memcpy(buf + url_len, (u8*)b64, b64_len);
    buf[url_len + b64_len] = '\0';
}

void qr_generate(ScoreEntry *const entry, const u16 x, const u16 y, const u16 index, const u8 pal)
{
    ScorePayload payload;

    // Prevent replay attacks
    payload.nonce = xorshift32();
    memcpy(&payload.entry, entry, sizeof(ScoreEntry));

    // Prepare Data Frame
    QRframe frame;
    frame.version = __MEGA_QR_VERSION__;
    frame.gameID = _game_config.gameID;

    // Generate random token for AES encryption
    nonce_generate(frame.iv, NONCE_SIZE_BYTES);

    // Prepare game private key
    u8 game_key_bytes[GAME_KEY_SIZE_BYTES];
    hex2bytes(_game_config.key, game_key_bytes);

    // Encrypt payload
    AES_CTR_encrypt(game_key_bytes, frame.iv, &payload, sizeof(ScorePayload), frame.payload);

    // Encode encrypted data as base64 string
    char b64String[B64_STR_SIZE];
    char text[TEXT_BUFFER_SIZE];
    bintob64(b64String, (u8 *)&frame, sizeof(QRframe));
    sprintf(text, url, b64String);
    concat_url_b64(text, TEXT_BUFFER_SIZE, url, b64String);

    // Generate the QRCode
    u8 QR[QRCODE_BUFFER_SIZE];
    u8 buffer[QRCODE_BUFFER_SIZE];
    QRCode_EncodeText(text, buffer, QR, QRCODE_ECC_LOW, QRCODE_MASK_5, FALSE);

    render_qr(QR, QRCode_GetSize(QR), index, pal, x, y);

#if defined (DEBUG)
    char * iv = bytes_to_hex(frame.iv, NONCE_SIZE_BYTES);
    kprintf("IV: %s", iv);
    free(iv);

    char * cipher = bytes_to_hex(frame.payload, sizeof(ScorePayload));
    kprintf("Cipher: %s", cipher);
    free(cipher);
#endif
}
