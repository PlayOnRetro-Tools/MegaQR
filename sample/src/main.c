#include <megaqr.h>

// Define a maximum of 32 achievements as a bitflag
#define bit(i) (1 << i)

#define ACHIEVEMENT_SAVED_PRINCESS bit(0)
#define ACHIEVEMENT_KILLED_TAILS   bit(1)
#define ACHIEVEMENT_END_GAME       bit(2)
#define ACHIEVEMENT_NO_MISS        bit(31)

#define QR_SCREEN_TILE_X 11
#define QR_SCREEN_TILE_Y 4

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
    // Set achievements as needed
    const u32 achievements = (ACHIEVEMENT_KILLED_TAILS | ACHIEVEMENT_SAVED_PRINCESS);

    // Create a new hi-score entry
    ScoreEntry entry = {
        .gameUIID = MEGA_QR_TEST_GAME_ID, // Game ID for testing, Actual ID will be provided by PlayOnRetro
        .device = DEVICE_TYPE_ROM,        // Digital version of the game.
        .score = U32_MAX,
        .achievementsUnlocked = achievements,
    };

    prepareScreen();

    // QR rendering needs to upload 16 tiles at the indicated vram index
    qr_generate(&entry, QR_SCREEN_TILE_X, QR_SCREEN_TILE_Y, TILE_USER_INDEX, PAL1);

    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
}
