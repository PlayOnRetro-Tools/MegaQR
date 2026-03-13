#pragma once

#include <genesis.h>

typedef struct
{
    u8         gameID;
    const char key[33]; // 16 bytes = 32 hex chars + null
} QRGameConfig;

/**
 * @Brief MegaQr Game configuration. All data will be provided by playonretro.
 * @param[in] uuid Unique game indentifier.
 * @param[in] key_hex Private game encryption key as 32 hex chars.
 */
#define QR_GAME_CONFIG(uuid, key_hex)   \
    const QRGameConfig _game_config = { \
        .gameID = (uuid),               \
        .key = (key_hex)}

/**
 * @brief Device type enumeration for score entries.
 */
typedef enum
{
    DEVICE_TYPE_CART = 0, /**< Physical cartridge device. */
    DEVICE_TYPE_ROM,      /**< ROM/emulated device. */
} Device_t;

/**
 * @brief Packed score entry structure for QR submission.
 */
typedef struct __attribute__((packed))
{
    u32 score;                /**< Actual game score. */
    u32 achievementsUnlocked; /**< Achievements unlocked as a bitflag. */
    u8  device;               /**< Device type (Device_t): cart or ROM. */
} ScoreEntry;

/**
 * @brief Generates QR code from score entry.
 * @param[in] entry Pointer to ScoreEntry to encode and render as QR.
 * @param[in] tilex Screen x position for rendering in tile coordinates.
 * @param[in] tiley Screen y position for rendering in tile coordinates.
 * @param[in] index VRAM index for qr tile data (16 tiles needed).
 * @param[in] pal Palette index to use. The QR code uploads its own palette.
 */
void qr_generate(ScoreEntry const *const entry, const u16 tilex, const u16 tiley, const u16 index, const u8 pal);
