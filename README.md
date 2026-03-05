# Mega Drive QR Code Library

This library enables QR code generation on Sega Mega Drive for leaderboard submissions. It encodes score data into scannable QR codes using SGDK, supporting scores and achievements.

## Features

- Generates QR codes from `ScoreEntry` structs with score, achievements, device type, and game ID.
- Renders QR codes directly to VDP plane A using black/white tiles from palette 0.

## Requirements

- SGDK (Sega Genesis Development Kit) for compilation and VDP access.
- Include headers: `megaqr.h`.
- Game ID from PlayOnRetro leaderboard service.


## ScoreEntry Structure

Defines leaderboard entry data for QR encoding.


| Field | Type | Description |
| :-- | :-- | :-- |
| `score` | u32 | Actual game score. |
| `achievementsUnlocked` | u32 | Bitflag for up to 32 achievements (use bit shifts like `1 << 0`). |
| `device` | `Devicetype` | `DEVICE_TYPE_CART` (0) or `DEVICE_TYPE_ROM` (1). |
| `game ID` | u8 | Unique game identifier from PlayOnRetro. |

`Devicetype` enum: `DEVICE_TYPE_CART=0`, `DEVICE_TYPE_ROM=1`.

## Usage

1. Define your game ID and achievements as bitflags.
2. Populate an `ScoreEntry` struct.
3. Set up VDP with a palette at index 0.
4. Invoke `qr_generate(&entry, tilex, tiley, vram_index)` to serialize, encode, generate, and draw QR to plane A.
5. Enter VBlank loop to display.

## Example

From `main.c` sample:

```c
#include <megaqr.h>

// Define a maximum of 32 achievements as a bitflag
#define bit(i) (1 << i)

#define ACHIEVEMENT_SAVED_PRINCESS bit(0)
#define ACHIEVEMENT_KILLED_TAILS   bit(1)
#define ACHIEVEMENT_END_GAME       bit(2)
#define ACHIEVEMENT_NO_MISS        bit(31)

#define QR_SCREEN_TILE_X 10
#define QR_SCREEN_TILE_Y 4

// Customize the screen for the QR code.
//
// The QR code gets rendered into the plane A using indexes 0 and 15 of the PAL0.
// Colors are expected to be black and white for compatilibity with scanning devices.
//
// This sample uses SGDKs default palette for simplicity.
//
static void prepareScreen(void)
{
    SYS_disableInts();

    SPR_reset();
    VDP_clearPlane(BG_A, 0);
    VDP_clearPlane(BG_B, 0);
    VDP_drawText("GENERATING QR CODE", 11, 1);

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
    qr_generate(&entry, QR_SCREEN_TILE_X, QR_SCREEN_TILE_Y, TILE_USER_INDEX);

    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
}

```

## Integration Notes

- Build with SGDK: Add header and library to your Makefile.
