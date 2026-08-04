#ifndef __CORE_UTILS_DRAW_STRING_TILES_H__
#define __CORE_UTILS_DRAW_STRING_TILES_H__

#include <stdint.h>

// shared menu attributes
#define MENU_ATTR (INK_WHITE | PAPER_BLACK)
#define MENU_ATTR_INVERSE (INK_BLACK | PAPER_WHITE)

/**
 * Draws a NUL-terminated string at (row, col) as a contiguous row of SP1
 * tiles.
 * @param row  Screen row (character cell).
 * @param col  Screen column of the first character.
 * @param attr SP1 attribute byte applied to every glyph (e.g. INK_WHITE | PAPER_BLACK).
 * @param str  NUL-terminated ASCII string. Each byte is interpreted as a
 *             tile index into the registered font range.
 *
 * @remarks: the caller MUST have already registered the font_1 ASCII glyphs
 * via sp1_TileEntry (using TILE_FONT_START / TILE_FONT_LEN from
 * features/ui/tile_registry.h and font_1 from data/fonts/font_1.h) before
 * invoking this helper. draw_string_tiles does NOT register tiles itself.
 */
extern void draw_string_tiles(uint8_t row, uint8_t col, uint8_t attr, const char *str) __z88dk_callee;

#endif // __CORE_UTILS_DRAW_STRING_TILES_H__
