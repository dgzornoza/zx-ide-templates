#ifndef TILE_REGISTRY_H
#define TILE_REGISTRY_H

// ============================================================================
// SP1 TILE REGISTRY (0 - 255)
// Central memory map for SP1 static graphics (Tiles).
// Keep here the registry of which IDs are free and which are occupied.
// ============================================================================

// --- ASCII SPACE (32 - 127) ---
// Reomended use ASCII indexes directly to easily print text.
#define TILE_FONT_NUMBERS_BASE '0' // = 48 (Occupies 48 to 57) (0-9 digits)

// --- CUSTOM / TILESET SPACE (128 - 255) ---
// Add here the identifiers for your backgrounds, HUD, etc.
// #define TILE_HUD_BASE            128  // Ex: 10 tiles (128-137)
// #define TILE_BACKGROUND_BASE     138  // Ex: 50 tiles (138-187)

#endif // TILE_REGISTRY_H
