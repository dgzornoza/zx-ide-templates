#include "hud_manager.h"
#include "../z88dk_headers.h"
#include "../data/hud/hud.h"
#include "../data/hud/hud-tiles.h"
#include "../core/utils/draw_map_region.h"
#include "../core/definitions.h"

// hud position
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0

void draw_hud(void)
{
    // draw hud.asm tiled data by configuring the global tiled_data_config
    DrawMapStaticRegionParams params;

    params.tiles_map = (const uint8_t *)hud;
    params.map_width = HUD_WIDTH;
    params.map_height = HUD_HEIGHT;
    params.tiles_data = hud_tiles;
    params.tiles_attributes = hud_tiles_attributes;
    params.map_x_offset = HUD_X_OFFSET;
    params.map_y_offset = HUD_Y_OFFSET;
    params.has_mirror = 0;

    // // mirror the hud on the right side, using the same tile data
    params.has_mirror = 1;
    params.map_x_offset = HUD_WIDTH;
    params.map_y_offset = HUD_Y_OFFSET;
    draw_map_static_region(&params);

    DrawMapRegionParams params1;

    params1.tiles_map = (const uint8_t *)hud;
    params1.map_width = HUD_WIDTH;
    params1.map_height = HUD_HEIGHT;
    params1.tiles_data = hud_tiles;
    params1.tiles_attributes = hud_tiles_attributes;
    params1.map_x_offset = HUD_X_OFFSET;
    params1.map_y_offset = HUD_Y_OFFSET;

    // draw_map_region(&params1);
}
