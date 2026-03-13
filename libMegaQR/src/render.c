#include "render.h"
#include "patterns.h"
#include "qrcode.h"

void render_qr(const u8 code[], const u16 size, const u16 index, const u8 pal, const u16 x, const u16 y)
{
    VDP_loadTileSet(&patternTiles, index, DMA_QUEUE);
    PAL_setPalette(pal, patternTilesPal.data, DMA_QUEUE);
    SYS_doVBlankProcess();

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
            const u8 a = !QRCode_GetModule(code, j, i);
            const u8 b = !QRCode_GetModule(code, j + 1, i);
            const u8 c = !QRCode_GetModule(code, j, i + 1);
            const u8 d = !QRCode_GetModule(code, j + 1, i + 1);

            // Map block to 8x8 tile
            const u8  nibble = (a << 3) | (b << 2) | (c << 1) | d;
            const u16 tile = TILE_ATTR_FULL(pal, TRUE, FALSE, FALSE, index + nibble);
            VDP_setTileMapXY(BG_A, tile, (x + 1) + (j >> 1), (y + 1) + (i >> 1));
        }
    }

    SYS_doVBlankProcess();
}
