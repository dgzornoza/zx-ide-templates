#include "player.h"
#include "../../../z88dk_headers.h"
#include "../../../data/player/player-spritesheet.h"
#include "../../../core/input/input_manager.h"

#define PLAYER_INITIAL_X 120
#define PLAYER_INITIAL_Y 160
#define PLAYER_SPEED_PIXELS 1
#define PLAYER_MIN_X 8
#define PLAYER_MAX_X 240
#define PLAYER_MIN_Y 24
#define PLAYER_MAX_Y 176

static struct sp1_ss *player_sprite = 0;
static uint8_t player_x = PLAYER_INITIAL_X;
static uint8_t player_y = PLAYER_INITIAL_Y;
static const struct sp1_Rect player_clip = {0, 0, 32, 24};

void player_init(uint8_t player_id) __z88dk_fastcall
{
    (void)player_id; // not used currently
    player_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, (int)sprites_player_spritesheet_ship_base, 0);
    sp1_AddColSpr(player_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);

    player_x = PLAYER_INITIAL_X;
    player_y = PLAYER_INITIAL_Y;
}

void player_update(uint8_t player_id) __z88dk_fastcall
{
    /* Per-frame tick for a player entity. Two phases, in this order:
     *   1. logic  - read input, advance position
     *   2. dirty-marker - sp1_MoveSprPix updates the SP1 update list so
     *      sp1_UpdateNow() (central loop) repaints the sprite this frame.
     * No separate _render callback; ordering between phases is local. */
    const uint8_t pressed = input_get_pressed(player_id);

    if ((pressed & INPUT_FLAG_LEFT) && player_x > PLAYER_MIN_X)
    {
        player_x -= PLAYER_SPEED_PIXELS;
    }

    if ((pressed & INPUT_FLAG_RIGHT) && player_x < PLAYER_MAX_X)
    {
        player_x += PLAYER_SPEED_PIXELS;
    }

    if ((pressed & INPUT_FLAG_UP) && player_y > PLAYER_MIN_Y)
    {
        player_y -= PLAYER_SPEED_PIXELS;
    }

    if ((pressed & INPUT_FLAG_DOWN) && player_y < PLAYER_MAX_Y)
    {
        player_y += PLAYER_SPEED_PIXELS;
    }

    sp1_MoveSprPix(player_sprite, (struct sp1_Rect *)&player_clip, 0, player_x, player_y);
}
