#include "../../z88dk_headers.h"
#include "../system_definitions.h"
#include "../game_definitions.h"
#include "sp1_utils.h"

void sp1_clear_screen_tiles(void) __z88dk_fastcall
{
    // set border as same colour
    zx_border(SCREEN_COLOR);

    const struct sp1_Rect game_area = {0, 0, SYS_SCREEN_CHARS_WIDTH, SYS_SCREEN_CHARS_HEIGHT};
    sp1_ClearRectInv(&game_area, SCREEN_COLOR, (uint16_t)' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR | SP1_RFLAG_SPRITE);
}

void draw_string(uint8_t row, uint8_t col, uint8_t attr, char *str) __z88dk_callee
{
    struct sp1_Rect rect = {row, col, 32, 1};
    struct sp1_pss ps = {&rect, SP1_PSSFLAG_INVALIDATE, 0, 0, SP1_ATTR_TRANS, attr, 0, 0};

    sp1_SetPrintPos(&ps, 0, 0);
    sp1_PrintString(&ps, str);
}
