#include <stdint.h>

#include "../../z88dk_headers.h"
#include "../system_definitions.h"
#include "draw_string_tiles.h"

void draw_string_tiles(uint8_t row, uint8_t col, uint8_t attr, const char *str) __z88dk_callee
{
    /* Measure the string length. */
    uint8_t len = 0u;
    while (str[len] != '\0')
    {
        len++;
    }

    /* Nothing to do for empty strings or for calls that start past the right edge of the visible screen. */
    if (len == 0u || col >= SCREEN_CHARS_WIDTH)
    {
        return;
    }

    /* Clamp the invalidated width to the visible columns */
    uint8_t max_len = (uint8_t)(SCREEN_CHARS_WIDTH - col);
    if (len > max_len)
    {
        len = max_len;
    }

    /* Mark the row segment dirty in one update-list entry. Subsequent
     * sp1_PrintAt calls only mutate the tile buffer without re-invalidating. */
    struct sp1_Rect rect;
    rect.row = row;
    rect.col = col;
    rect.width = len;
    rect.height = 1u;
    sp1_Invalidate(&rect);

    /* Write glyphs to the tile buffe */
    for (uint8_t i = 0u; i < len; i++)
    {
        sp1_PrintAt(row, (uint8_t)(col + i), attr, (uint8_t)str[i]);
    }
}
