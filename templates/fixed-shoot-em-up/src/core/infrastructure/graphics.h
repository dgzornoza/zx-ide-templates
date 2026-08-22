#ifndef __CORE_INFRASTRUCTURE_TILE_BUFFER_H__
#define __CORE_INFRASTRUCTURE_TILE_BUFFER_H__

#include <stdint.h>

// =============================================================================
// TILE BUFFER — SP1 tile buffer graphics.
//
// The buffer holds the 8-byte graphic and 1-byte attribute for every SP1
// tile slot (0-255). sp1_TileEntry(slot, ptr) always points into this
// buffer, SP1 caches the graphic on first draw and re-reads from the
// pointer on subsequent invalidations.
//
// Memory footprint (static, lives in BSS):
//   tile_gfx  : 256 * 8 = 2048 bytes
//   tile_attr : 256     =  256 bytes
//
// tile_set_compressed_range decompresses directly into screen pixel memory
// (SYS_SCREEN_PIXEL_ADDR, 6144 bytes) instead of a dedicated scratch buffer.
// See the function's documentation for the timing requirement.
//
// Each scene declares which slots it uses and fills them via
// tile_set / tile_set_range / tile_set_compressed_range.
// =============================================================================

/** 8-byte graphics for every SP1 tile slot, indexed by slot number. */
extern uint8_t tile_gfx[256][8];

/** Attribute byte for every SP1 tile slot, indexed by slot number. */
extern uint8_t tile_attr[256];

/**
 * Register N consecutive slots from a ZX0-compressed payload in tiles buffer.
 * Payload format (after decompression): N*8 bytes of graphics followed by
 * N bytes of attributes. Matches the layout exported by zx-ide Tiled toolchain
 * for tilesets with per-tile attributes (HUD, level backgrounds, etc.).
 *
 * Implementation note: decompresses directly into screen pixel memory
 * (SYS_SCREEN_PIXEL_ADDR, 6144 bytes). LIFECYCLE REQUIREMENT: must be
 * called AFTER sp1_ClearRectInv() and BEFORE the next sp1_UpdateNow().
 * The dispatcher guarantees this order on scene transitions. Bytes touched
 * in screen memory are stale for at most one frame and are overwritten by
 * the next sp1_UpdateNow() repainting every invalidated cell from tile_gfx.
 *
 * @param start_slot  First SP1 slot 0-255, must satisfy start+N <= 256.
 * @param count       Number of slots to write (1-256).
 * @param compressed  ZX0-compressed payload (N*8 + N bytes when raw).
 */
void graphics_register_tile_compressed_range(uint8_t start_slot, uint8_t count, const uint8_t *compressed);

#endif // __CORE_INFRASTRUCTURE_TILE_BUFFER_H__
