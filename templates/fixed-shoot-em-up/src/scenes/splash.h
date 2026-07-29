#ifndef __SCENES_SPLASH_H__
#define __SCENES_SPLASH_H__

/*
 * splash_scene_init - One-shot setup run BEFORE the central dispatch loop:
 * registers the font_1 ASCII tiles with SP1 and prints the "PULSA UNA TECLA"
 * prompt centered on display row 20 with the FLASH attribute bit set so the
 * ULA drives the blink at ~2 Hz with zero per-frame CPU work. SP1 has already
 * been initialized at this point; the loaded load-screen.scr art survives
 * because sp1_Initialize was called WITHOUT OVERWRITE_DFILE/OVERWRITE_TILES.
 */
void splash_scene_init(void);

/*
 * splash_scene_update - Per-frame keypoll. Samples in_inkey() (lowercase;
 * sdcc_iy clib symbol from <input/input_zx.h>) and detects a rising edge
 * against a prev = 0xFFu sentinel. On the edge, sets game_state = STATE_MENU.
 * A key held at boot cannot satisfy the first edge because prev starts at 0xFF.
 */
void splash_scene_update(void);

/*
 * splash_scene_render - No-op. Kept in the API to conform to the init/update/render
 * scene triad used by level1 and score.
 */
void splash_scene_render(void);

#endif /* __SCENES_SPLASH_H__ */
