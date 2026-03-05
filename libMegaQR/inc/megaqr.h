#pragma once

#include <genesis.h>

/*
 * @brief Sample unique game id for testing purposes.
 */
#define MEGA_QR_TEST_GAME_ID    64

/**
 * @brief Device type enumeration for score entries.
 */
typedef enum
{
    DEVICE_TYPE_CART = 0,  /**< Physical cartridge device. */
    DEVICE_TYPE_ROM,       /**< ROM/emulated device. */
} Device_t;

/**
 * @brief Packed score entry structure for QR submission.
 */
#pragma pack(push, 1)
typedef struct
{
    u8 gameUIID;               /**< Unique game identifier (provided by PlayOnRetro). */
    u32 score;                 /**< Actual game score. */
    u32 achievementsUnlocked;  /**< Achievements unlocked as a bitflag. */
    u8 device;                 /**< Device type (Device_t): cart or ROM. */
} ScoreEntry;
#pragma pack(pop)

/**
 * @brief Generates QR code from score entry.
 * @param[in] entry Pointer to ScoreEntry to encode and render as QR.
 * @param[in] tilex Screen x position for rendering in tile coordinates.
 * @param[in] tiley Screen y position for rendering in tile coordinates.
 * @param[in] index VRAM index for qr tile data (16 tiles needed).
 */
void qr_generate(ScoreEntry const *const entry, const u16 tilex, const u16 tiley, const u16 index);
