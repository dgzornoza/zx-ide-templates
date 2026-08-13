#include "../../z88dk_headers.h"
#include "../system_definitions.h"
#include "../game_definitions.h"
#include "sp1_utils.h"

void sp1_clear_tile_buffer(void) __z88dk_fastcall
{
    const struct sp1_Rect game_area = {0, 0, SYS_SCREEN_CHARS_WIDTH, SYS_SCREEN_CHARS_HEIGHT};

    sp1_ClearRect(&game_area, SCREEN_COLOR, (uint16_t)' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
}
