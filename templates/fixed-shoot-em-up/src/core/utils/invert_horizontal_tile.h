#ifndef INVERT_TILE_ASM_H
#define INVERT_TILE_ASM_H

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
} TileMirrorArgs;

extern void invert_horizontal_tile_struct(TileMirrorArgs *args) __z88dk_fastcall;

#endif // INVERT_TILE_ASM_H
