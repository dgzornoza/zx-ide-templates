#include "hud.h"
#include "../../../z88dk_headers.h"
#include "../../../core/infrastructure/graphics.h"
#include "../../../data/hud/hud-tiles.h"
#include "../../../core/utils/draw_map_region.h"
#include "../../../data/hud/hud-map.h"
#include "tile_registry.h"
#include "../../../core/utils/zx0.h"

// hud position
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0
#define HUD_WIDTH 32
#define HUD_HEIGHT 4

void hud_init(void) __z88dk_fastcall
{
    /* Decompress the HUD tileset into the shared tile buffer and register
     * each slot with SP1. After this call, tile_gfx[sp1_slot] holds the bitmap
     * and tile_attr[sp1_slot] holds the colour attribute for slot sp1_slot. */
    graphics_register_tile_compressed_range(TILE_HUD_START, HUD_TILES_TILES_COUNT, hud_tiles_compressed);

    /* Draw the HUD region from the registered tile slots.*/
    draw_map_config.start_tile_entry_index = TILE_HUD_START;
    draw_map_config.tiles_map = (const uint8_t *)hud_map;
    draw_map_config.map_width = HUD_WIDTH;
    draw_map_config.map_height = HUD_HEIGHT;
    draw_map_config.map_x_offset = HUD_X_OFFSET;
    draw_map_config.map_y_offset = HUD_Y_OFFSET;
    draw_map_region();
}
