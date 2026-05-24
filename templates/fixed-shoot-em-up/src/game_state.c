#include "game_state.h"

// Define and initialize the actual storage for game variables
GameState game_state = STATE_MENU;
uint16_t game_score = 0;
uint8_t game_lives = 3;

void game_state_init(void)
{
    game_state = STATE_PLAYING;
    game_score = 0;
    game_lives = 3;
}
