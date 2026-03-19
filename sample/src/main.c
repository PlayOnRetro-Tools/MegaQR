#include <megaqr.h>

// Define a maximum of 32 achievements
#define ACHIEVEMENT_SAVED_PRINCESS ACHIEVEMENT_BIT(0)
#define ACHIEVEMENT_KILLED_TAILS   ACHIEVEMENT_BIT(1)
#define ACHIEVEMENT_END_GAME       ACHIEVEMENT_BIT(2)
#define ACHIEVEMENT_NO_MISS        ACHIEVEMENT_BIT(31)

#define QR_SCREEN_TILE_X ((320 / 2) - (128 / 2)) / 8
#define QR_SCREEN_TILE_Y 4

// Declare your game ID and private 16 byte key as 32 char hex string
//
// Provided by playonretro to developers
//
QR_GAME_CONFIG(12, "1e67c3fcbfa32fe5dd73a7e8c0e7d9bb");

// Customize the screen for the QR code.
//
// The QR code gets rendered into the plane A (128x128 pixels) with its own palette.
//
static void prepareScreen(void)
{
    SYS_disableInts();
    SPR_reset();
    VDP_clearPlane(BG_A, 0);
    VDP_clearPlane(BG_B, 0);
    VDP_setVerticalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_B, 0);
    VDP_setPlaneSize(64, 32, TRUE);
    SYS_enableInts();

    SYS_doVBlankProcess();
}

int main(bool hard_reset)
{
    // Set achievements
    const u32 achievements = (ACHIEVEMENT_KILLED_TAILS | ACHIEVEMENT_SAVED_PRINCESS);

    // Create a new hi-score entry
    ScoreEntry entry = {
        .device = DEVICE_TYPE_ROM, // Digital version of the game.
        .score = U32_MAX,
        .achievements = achievements,
    };

    prepareScreen();

    // QR rendering needs to upload 16 tiles at the indicated vram index
    megaqr_generate(&entry, QR_SCREEN_TILE_X, QR_SCREEN_TILE_Y, /* vram index = */ TILE_USER_INDEX, PAL1);

    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
}
