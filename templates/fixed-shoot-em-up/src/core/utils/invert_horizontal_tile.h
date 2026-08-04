#ifndef __CORE_UTILS_INVERT_HORIZONTAL_TILE_H__
#define __CORE_UTILS_INVERT_HORIZONTAL_TILE_H__

#include <stdint.h>

/**
 * Arguments structure for horizontal tile mirroring.
 * This structure is used to pass parameters to the function that performs the tile mirroring.
 */
typedef struct
{
    // Pointer to the tile data to be mirrored (8 bytes, one per row).
    const uint8_t *source;
    // Pointer to the destination buffer where the mirrored tile will be stored (8 bytes, one per row).
    uint8_t *dest;
} TileMirrorParams;

/**
 * Inverts a tile's bitmap horizontally.
 * This function takes a source tile (8 bytes) and produces a horizontally flipped version in the destination buffer.
 * @param params A pointer to a TileMirrorParams structure containing the source and destination pointers.
 */
extern void invert_horizontal_tile(TileMirrorParams *params) __z88dk_fastcall;

#endif // __CORE_UTILS_INVERT_HORIZONTAL_TILE_H
