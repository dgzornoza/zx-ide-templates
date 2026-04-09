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
    DrawTiledParams params;
    params.tile_map = (const uint8_t *)hud;
    params.map_width = HUD_WIDTH;
    params.map_height = HUD_HEIGHT;
    params.tiles_data = hud_tiles;
    params.tiles_attributes = hud_tiles_attributes;
    params.map_x_offset = HUD_X_OFFSET;
    params.map_y_offset = HUD_Y_OFFSET;
    params.has_mirror = 0;

    draw_map_region(&params);
}
