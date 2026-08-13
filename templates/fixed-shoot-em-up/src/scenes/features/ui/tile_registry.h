#ifndef __SCENES_FEATURES_UI_TILE_REGISTRY_H__
#define __SCENES_FEATURES_UI_TILE_REGISTRY_H__

// ============================================================================
// SP1 TILE REGISTRY (0 - 255)
// Central memory map for SP1 static graphics (Tiles).
// Keep here the registry of which IDs are free and which are occupied.
// ============================================================================

// --- ASCII SPACE (32 - 127) ---
// Reomended use ASCII indexes directly to easily print text.
#define TILE_FONT_START ' '       // = 32 start font pointer
#define TILE_FONT_LEN 127u - 32u  // font tiles (32 to 127)
#define TILE_FONT_NUMBERS '0'     // = 48 start numbers
#define TILE_FONT_NUMBERS_LEN 10u // 10 digits
#define TILE_FONT_UPPER 'A'       // = 65 start upercase letters
#define TILE_FONT_UPPER_LEN 26u   // 26 letters
#define TILE_FONT_LOWER 'a'       // = 97 start lowercase letters
#define TILE_FONT_LOWER_LEN 26u   // 26 letters

// --- CUSTOM / TILESET SPACE (128 - 255) ---
// Add here the identifiers for your backgrounds, HUD, etc.
#define TILE_HUD_BASE 128 // 106 tiles (128-233)
// #define TILE_BACKGROUND_BASE     138  // Ex: 50 tiles (138-187)

#endif // __SCENES_FEATURES_UI_TILE_REGISTRY_H__
