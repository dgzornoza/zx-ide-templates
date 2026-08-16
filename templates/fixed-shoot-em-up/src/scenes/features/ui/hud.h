#ifndef __SCENES_FEATURES_UI_HUD_H__
#define __SCENES_FEATURES_UI_HUD_H__

/**
 * Init lifecycle function
 *
 * Initializes the HUD entity. Must be called exactly once from the
 * owning scene's init.
 *
 * The HUD is static and generally does not require repainting,
 * so currently only an initialization function is necessary to render it.
 */
void hud_init(void) __z88dk_fastcall;

#endif // __SCENES_FEATURES_UI_HUD_H__
