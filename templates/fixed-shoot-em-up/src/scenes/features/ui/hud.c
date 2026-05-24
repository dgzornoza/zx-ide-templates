#include "hud.h"
#include "../../../z88dk_headers.h"
#include "../../../data/hud/hud-map.h"
#include "../../../data/hud/hud-tiles.h"
#include "../../../core/utils/draw_map_region.h"
#include "../../../core/definitions.h"

// hud position
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0

void hud_render(void)
{
    draw_map_config.tiles_map = (const uint8_t *)hud_map;
    draw_map_config.map_width = HUD_MAP_WIDTH;
    draw_map_config.map_height = HUD_MAP_HEIGHT;
    draw_map_config.tiles_data = hud_tiles;
    draw_map_config.tiles_attributes = hud_tiles_attributes;
    draw_map_config.map_x_offset = HUD_X_OFFSET;
    draw_map_config.map_y_offset = HUD_Y_OFFSET;
    draw_map_region();

    // mirror the hud on the right side, using the same tile data
    draw_map_config.map_x_offset = HUD_MAP_WIDTH;
    draw_map_static_region_mirrored();
}
