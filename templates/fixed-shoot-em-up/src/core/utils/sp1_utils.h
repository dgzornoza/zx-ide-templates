#ifndef __CORE_UTILS_SP1_UTILS_H__
#define __CORE_UTILS_SP1_UTILS_H__

/**
 * Clears the SP1 tiles for the entire visible screen.
 *
 * Fills and invalidate every (row, col) cell with the blank tile (' ') at SCREEN_COLOR
 * so the cleared state reaches screen memory in the same call.
 * After this returns SP1 has no memory of any prior scene's tiles.
 */
void sp1_clear_screen_tiles(void) __z88dk_fastcall;

/**
 * Draws a NUL-terminated string at (row, col) as a contiguous row of SP1
 * tiles.
 * @param row  Screen row (character cell).
 * @param col  Screen column of the first character.
 * @param attr SP1 attribute byte applied to every glyph (e.g. INK_WHITE | PAPER_BLACK).
 * @param str  NUL-terminated ASCII string. Each byte is interpreted as a
 *             tile index into the registered font range.
 *
 * @remarks: the caller can register custom font_1 ASCII glyphs
 * via sp1_TileEntry, otherwise default font is used.
 */
void sp1_draw_string(uint8_t row, uint8_t col, uint8_t attr, const char *str) __z88dk_callee;

#endif // __CORE_UTILS_SP1_UTILS_H__
