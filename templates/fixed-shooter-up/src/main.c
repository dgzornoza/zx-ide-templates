#include "./z88dk_headers.h"
#include "infrastructure/isr.h"
#include "data/tiles.h"

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
    for (int i = 0; i < 224; i++)
    {
        sp1_TileEntry(i, tiles_tiles + i * 8);
    }

    sp1_TileEntry(54, temp);

    // Pintar una linea de suelo de hierba en la fila 22 mediante un bucle
    uint8_t index = 33;
    for (uint8_t x = 32; x--;)
    {
        // sp1_PrintAt(22, x, INK_GREEN | PAPER_BLACK, index);
        for (uint8_t y = 24; y--;)
        {
            sp1_PrintAt(y, x, INK_GREEN | PAPER_BLACK, index);
        }

        index++;
        if (index == 255)
        {
            index = 33;
        }
    }

    // main app loop
    while (1)
    {
        // Fuerza el redibujado de las celdas invalidadas
        sp1_UpdateNow();
    }
}
