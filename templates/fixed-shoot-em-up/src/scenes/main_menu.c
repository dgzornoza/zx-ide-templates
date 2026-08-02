/*
 * main_menu.c - Main menu scene for ZX-IDE fixed-shoot-em-up template.
 * Runs AFTER splash transitions game_state to STATE_MENU.
 */

#include <stdint.h>
#include <intrinsic.h>

#include "../z88dk_headers.h"
#include "features/ui/tile_registry.h"
#include "../data/fonts/font_1.h"
#include "../game_state.h"
#include "../core/system_definitions.h"
#include "../core/utils/draw_string_tiles.h"

/* Menu entries */
#define MENU_LEFT_COL 10u

#define MENU_LABEL_0 "0 Jugar"
#define MENU_LABEL_1 "1 Keyboard"
#define MENU_LABEL_2 "2 Kempston"
#define MENU_LABEL_3 "3 Sinclair"

/* Copyright block */
#define COPYRIGHT_LABEL_0 "(C) 2026 David Gonzalez"
#define COPYRIGHT_LABEL_1 "fixed-shoot-em-up"
#define COPYRIGHT_LABEL_2 "powered by Zx-Ide"

/* -------------------------------------------------------------------------
 * Lifecycle
 * ------------------------------------------------------------------------- */

void main_menu_scene_init(void) __z88dk_fastcall
{
    zx_border(PAPER_BLACK);
    zx_cls(PAPER_BLACK);

    // /* Menu entries. */
    draw_string_tiles(4u, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_0);
    draw_string_tiles(7u, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_1);
    draw_string_tiles(9u, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_2);
    draw_string_tiles(11u, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_3);

    // /* Copyright block.*/
    draw_string_tiles(18u, 0u, INK_WHITE | PAPER_BLACK, COPYRIGHT_LABEL_0);
    draw_string_tiles(20u, 0u, INK_WHITE | PAPER_BLACK, COPYRIGHT_LABEL_1);
    draw_string_tiles(22u, 15u, INK_WHITE | PAPER_BLACK, COPYRIGHT_LABEL_2);
}

void main_menu_scene_update(void) __z88dk_fastcall
{
    // TODO: falta implementar
}
