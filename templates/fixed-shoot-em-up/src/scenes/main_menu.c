/*
 * main_menu.c - Main menu scene for ZX-IDE fixed-shoot-em-up template.
 *
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
#include "../core/utils/sp1_clear_screen.h"

/* -------------------------------------------------------------------------
 * Display geometry
 * ------------------------------------------------------------------------- */

/* Menu entries */
#define MENU_LEFT_COL 8u

#define MENU_LABEL_0 "0 Jugar"
#define MENU_LABEL_0_ROW 4u
#define MENU_LABEL_1 "1 Keyboard"
#define MENU_LABEL_1_ROW 7u
#define MENU_LABEL_2 "2 Kempston"
#define MENU_LABEL_2_ROW 9u
#define MENU_LABEL_3 "3 Sinclair"
#define MENU_LABEL_3_ROW 11u

/* Copyright block */
#define COPYRIGHT_COL 0u

#define COPYRIGHT_LABEL_0 "(C) 2026 David Gonzalez Zornoza"
#define COPYRIGHT_LABEL_0_ROW 18u
#define COPYRIGHT_LABEL_1 "fixed-shoot-em-up"
#define COPYRIGHT_LABEL_1_ROW 20u
#define COPYRIGHT_LABEL_2 "powered by ZxIde"
#define COPYRIGHT_LABEL_2_ROW 22u

/* -------------------------------------------------------------------------
 * Lifecycle
 * ------------------------------------------------------------------------- */

void main_menu_scene_init(void) __z88dk_fastcall
{
    zx_border(PAPER_BLACK);
    zx_cls(PAPER_BLACK);

    /* Menu entries. */
    draw_string_tiles(MENU_LABEL_0_ROW, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_0);
    draw_string_tiles(MENU_LABEL_1_ROW, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_1);
    draw_string_tiles(MENU_LABEL_2_ROW, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_2);
    draw_string_tiles(MENU_LABEL_3_ROW, MENU_LEFT_COL, INK_WHITE | PAPER_BLACK, MENU_LABEL_3);

    /* Copyright block. NOTE: COPYRIGHT_TEXT_2 is 34 chars and exceeds the
     * 32-column display; the trailing "de" will be clipped off-screen. */
    draw_string_tiles(COPYRIGHT_LABEL_0_ROW, COPYRIGHT_COL, INK_WHITE | PAPER_BLACK, COPYRIGHT_LABEL_0);
    draw_string_tiles(COPYRIGHT_LABEL_1_ROW, COPYRIGHT_COL, INK_WHITE | PAPER_BLACK, COPYRIGHT_LABEL_1);
    draw_string_tiles(COPYRIGHT_LABEL_2_ROW, COPYRIGHT_COL, INK_WHITE | PAPER_BLACK, COPYRIGHT_LABEL_2);
}

void main_menu_scene_update(void) __z88dk_fastcall
{
    // TODO: falta implementar
}
