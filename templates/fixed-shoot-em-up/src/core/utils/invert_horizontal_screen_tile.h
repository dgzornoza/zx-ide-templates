#ifndef INVERT_TILE_ASM_H
#define INVERT_TILE_ASM_H

#include <stdint.h>

/**
 * @brief Inverts a ZX Spectrum 8x8 tile horizontally in-place.
 *
 * Each row of the tile (8 bytes) is replaced by its horizontally mirrored value using a lookup table.
 *
 * @param address Pointer to the screen tile data (8 bytes). The operation is performed in-place.
 */
void invert_horizontal_screen_tile(void *address) __z88dk_fastcall;

#endif // INVERT_TILE_ASM_H
