#include "hud.h"
#include "../../../z88dk_headers.h"
#include "../../../data/hud/hud-tiles.h"
#include "../../../core/utils/draw_map_region.h"
#include "../../../core/system_definitions.h"
#include "../../../core/utils/zx0.h"
#include "../../../data/hud/hud-map.h"
#include "tile_registry.h"

// hud position
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0
#define HUD_WIDTH 32
#define HUD_HEIGHT 4

void hud_init(void) __z88dk_fastcall
{
    /* Decompress hud tiles in screen */
    dzx0_turbo((uint8_t *)hud_tiles_compressed, (void *)SYS_SCREEN_PIXEL_ADDR);

    // dzx0_standard((uint8_t *)hud_map_compressed, (void *)SYS_SCREEN_PIXEL_ADDR + HUD_TILES_TOTAL_SIZE);

    /* Sets up draw_map_config from the HUD asset headers and draws the HUD
     * in the top-left region of the screen.
     */
    draw_map_config.sp1_start_tile_entry_index = TILE_HUD_START;
    draw_map_config.tiles_map = (const uint8_t *)hud_map;
    draw_map_config.map_width = HUD_WIDTH;
    draw_map_config.map_height = HUD_HEIGHT;
    draw_map_config.tiles_data = (const uint8_t *)SYS_SCREEN_PIXEL_ADDR;
    draw_map_config.tiles_attributes = (const uint8_t *)SYS_SCREEN_PIXEL_ADDR + HUD_TILES_PIXMAP_SIZE;
    draw_map_config.map_x_offset = HUD_X_OFFSET;
    draw_map_config.map_y_offset = HUD_Y_OFFSET;
    draw_map_region();
}
