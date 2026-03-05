#include "qr.h"
#include "qrcode.h"

extern const u32 qr_tiledata[];

// Map hexcode to tile pattern index in vram
static const u8 pattern_lut[16] = {
    0,  // 0000
    1,  // 0001
    2,  // 0010
    6,  // 0011
    4,  // 0100
    5,  // 0101
    15, // 0110
    11, // 0111
    3,  // 1000
    14, // 1001
    7,  // 1010
    12, // 1011
    8,  // 1100
    10, // 1101
    9,  // 1110
    13, // 1111
};

void QR_drawQrCode(char *msg, u16 xx, u16 yy, const u16 index)
{
    VDP_loadTileData((const u32 *)qr_tiledata, index, 16, DMA);

    QRCode qrcode;

    u8 qrcodeData[qrcode_getBufferSize(5)];
    qrcode_initText(&qrcode, qrcodeData, 5, ECC_LOW, msg);

    VDP_setTileMapXY(BG_A, index + 1, xx, yy);

    for (u16 i = 2; i <= qrcode.size + 1; i += 2)
    {
        VDP_setTileMapXY(BG_A, index + 5, xx, yy + i / 2);
        VDP_setTileMapXY(BG_A, index + 6, xx + i / 2, yy);
    }

    for (u16 y = 0; y < qrcode.size; y += 2)
    {
        for (u16 x = 0; x < qrcode.size; x += 2)
        {
            const u32 a = !qrcode_getModule(&qrcode, x, y);
            const u32 b = !qrcode_getModule(&qrcode, x + 1, y);
            const u32 c = !qrcode_getModule(&qrcode, x, y + 1);
            const u32 d = !qrcode_getModule(&qrcode, x + 1, y + 1);
            const u8  nibble = (a << 3) | (b << 2) | (c << 1) | d;
            const u8  pattern = pattern_lut[nibble];

            VDP_setTileMapXY(BG_A, index + pattern, (xx + 1) + (x >> 1), (yy + 1) + (y >> 1));
        }
    }
}
