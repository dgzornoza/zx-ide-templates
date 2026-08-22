#include "../../z88dk_headers.h"
#include "graphics.h"
#include "../system_definitions.h"
#include "../utils/zx0.h"
#include <string.h>

// Buffer graphics, Lives in BSS, zero-initialised on boot so unused slots read as 0x00.
uint8_t tile_gfx[256][8];
uint8_t tile_attr[256];

void graphics_register_tile_compressed_range(uint8_t start_slot, uint8_t count, const uint8_t *compressed)
{
    // Decompress directly into screen pixel memory. The screen is 6144
    // bytes, sp1_TileEntry can register 256 slots, so (256 * 8) + 256 bytes fits in screen memory.
    uint8_t *screen = (uint8_t *)SYS_SCREEN_PIXEL_ADDR;
    dzx0_turbo((void *)compressed, screen);

    memcpy(&tile_gfx[start_slot][0], screen, (size_t)count * 8u);
    memcpy(&tile_attr[start_slot], screen + (size_t)count * 8u, count);

    for (uint8_t i = 0; i < count; i++)
    {
        sp1_TileEntry(start_slot + i, tile_gfx[start_slot + i]);
    }
}
