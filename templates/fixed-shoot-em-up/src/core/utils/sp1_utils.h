#ifndef __CORE_UTILS_SP1_UTILS_H__
#define __CORE_UTILS_SP1_UTILS_H__

/**
 * Clears the SP1 tile buffer for the entire visible screen.
 *
 * Fills and invalidate every (row, col) cell with the blank tile (' ') at SCREEN_COLOR
 * so the cleared state reaches screen memory in the same call.
 * After this returns SP1 has no memory of any prior scene's tiles.
 */
extern void sp1_clear_tile_buffer(void) __z88dk_fastcall;

#endif // __CORE_UTILS_SP1_UTILS_H__
