#include "./z88dk_headers.h"
#include "infrastructure/isr.h"
#include "data/tiles.h"
#include <string.h>

const struct sp1_Rect game_area = {0, 0, 32, 24};

uint8_t temp[] = {
    0b11111111,
    0b11111111,
    0b11001111,
    0b11001111,
    0b11001111,
    0b11001111,
    0b11111111,
    0b11111111,
};

uint8_t temp2[] = {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
};

/** Main app entry point */
int main(void)
{
    im2_setup();

    // Inicializar el motor SP1 con tablas de rotacion y limpieza de pantalla
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_BLACK | PAPER_BLACK, ' ');

    // Marcar toda la pantalla como "sucia" para el primer renderizado completo
    sp1_Invalidate(&game_area);

    // for (int i = 33; i < 256; i++)
    // {
    //     sp1_TileEntry(45 + i, tiles_tiles + i * 8);
    // }
    // for (int i = 0; i < 256; i++)
    // {
    //     const uint8_t *tile = tiles_tiles;
    //     // if (i == 511)
    //     // {
    //     //     tile = tile + 8;
    //     // }

    //     if (i == 255)
    //     {
    //         tile = tile + 8;
    //     }

    //     // sp1_TileEntry(i, (tiles_tiles + i * 8));
    //     sp1_TileEntry(i, tile);
    // }

    sp1_TileEntry(1, temp);
    sp1_PrintAt(10, 10, INK_GREEN | PAPER_BLACK, 1);
    sp1_PrintAt(11, 10, INK_GREEN | PAPER_BLACK, 1);

    // Pintar una linea de suelo de hierba en la fila 22 mediante un bucle
    // int index = 0;
    // for (uint8_t y = 0; y < 24; y++)
    // {
    //     // sp1_PrintAt(22, x, INK_GREEN | PAPER_BLACK, index);
    //     for (uint8_t x = 0; x < 32; x++)
    //     {
    //         sp1_PrintAt(y, x, INK_GREEN | PAPER_BLACK, index);

    //         index++;
    //         if (index == 256)
    //         {
    //             index = 0;
    //         }
    //     }
    // }

    // main app loop
    while (1)
    {
        // Fuerza el redibujado de las celdas invalidadas
        // memcpy(temp, temp2, sizeof(temp2));
        sp1_UpdateNow();
        z80_delay_ms(200);
        memcpy(temp, temp2, sizeof(temp2));
        // z80_delay_ms(200);
        sp1_PrintAt(12, 10, INK_GREEN | PAPER_BLACK, 1);
        sp1_Invalidate(&game_area);
    }
}
