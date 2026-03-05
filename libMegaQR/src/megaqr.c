#include <genesis.h>

#include "base64.h"
#include "megaqr.h"
#include "patterns.h"
#include "qrcode.h"
#include "rc4.h"

#define SECRET_KEY      "secretcodeporkey"
#define SECRET_KEY_SIZE 16

#define QRCODE_BUFFER_SIZE 256
#define B64_STR_SIZE       32
#define TEXT_BUFFER_SIZE   128

static const char *url = "http://192.168.1.38:5000/submit/<%s>";

static void renderQRCode(const u8 code[], const u16 size, const u16 index, const u8 pal, const u16 x, const u16 y)
{
    // Top left corner
    const u16 tile = TILE_ATTR_FULL(pal, TRUE, FALSE, FALSE, index + 1);
    VDP_setTileMapXY(BG_A, tile, x, y);

    for (u16 i = 2; i <= size + 1; i += 2)
    {
        // Left-most Column
        u16 tile = TILE_ATTR_FULL(pal, TRUE, FALSE, FALSE, index + 5);
        VDP_setTileMapXY(BG_A, tile, x, y + (i >> 1));

        // Top row
        tile = TILE_ATTR_FULL(pal, TRUE, FALSE, FALSE, index + 3);
        VDP_setTileMapXY(BG_A, tile, x + (i >> 1), y);
    }

    // Data
    for (u16 i = 0; i < size; i += 2)
    {
        for (u16 j = 0; j < size; j += 2)
        {
            // Get a 2x2 bit block from the code
            const u32 a = !QRCode_GetModule(code, j, i);
            const u32 b = !QRCode_GetModule(code, j + 1, i);
            const u32 c = !QRCode_GetModule(code, j, i + 1);
            const u32 d = !QRCode_GetModule(code, j + 1, i + 1);

            // Map block to 8x8 tile
            const u8  nibble = (a << 3) | (b << 2) | (c << 1) | d;
            const u16 tile = TILE_ATTR_FULL(pal, TRUE, FALSE, FALSE, index + nibble);
            VDP_setTileMapXY(BG_A, tile, (x + 1) + (j >> 1), (y + 1) + (i >> 1));
        }
    }

    SYS_doVBlankProcess();
}

void qr_generate(ScoreEntry const *const entry, const u16 x, const u16 y, const u16 index, const u8 pal)
{
    VDP_loadTileSet(&patternTiles, index, DMA_QUEUE);
    PAL_setPalette(pal, patternTilesPal.data, DMA_QUEUE);
    SYS_doVBlankProcess();

    // Don't overwrite original entry data
    ScoreEntry cypher;
    memcpy(&cypher, entry, sizeof(ScoreEntry));

    rc4_ctx ctx;
    rc4_init(&ctx, (u8 *)SECRET_KEY, SECRET_KEY_SIZE);
    rc4_crypt(&ctx, (u8 *)&cypher, sizeof(ScoreEntry));

    // Encode encrypted data as base64 string
    char b64String[B64_STR_SIZE];
    char text[TEXT_BUFFER_SIZE];
    bintob64(b64String, (u8 *)&cypher, sizeof(ScoreEntry));
    sprintf(text, url, b64String);

    u8 QR[QRCODE_BUFFER_SIZE];
    u8 buffer[QRCODE_BUFFER_SIZE];
    QRCode_EncodeText(text, buffer, QR, QRCODE_ECC_LOW, QRCODE_MASK_3, FALSE);

    renderQRCode(QR, QRCode_GetSize(QR), index, pal, x, y);
}
