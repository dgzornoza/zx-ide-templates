#include "hud_manager.h"
#include "../z88dk_headers.h"
#include "../data/hud/hud.h"
#include "../data/hud/hud-tiles.h"
#include "../core/utils/draw_tiled_data.h"
#include "../core/definitions.h"

// hud position
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0

void draw_hud(void)
{
    // draw hud.asm tiled data by configuring the global tiled_data_config
    draw_tiled_config.tile_map = (const uint8_t *)hud;
    draw_tiled_config.map_width = HUD_WIDTH;
    draw_tiled_config.map_height = HUD_HEIGHT;
    draw_tiled_config.tiles_data = hud_tiles;
    draw_tiled_config.tiles_attributes = hud_tiles_attributes;
    draw_tiled_config.map_x_offset = HUD_X_OFFSET;
    draw_tiled_config.map_y_offset = HUD_Y_OFFSET;
    draw_tiled_config.mirror = 1;

    draw_tiled_data();

    // sp1_UpdateNow();
}
