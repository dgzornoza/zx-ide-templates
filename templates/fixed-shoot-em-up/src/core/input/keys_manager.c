#include "keys_manager.h"

static uint8_t left_pressed = 0;
static uint8_t right_pressed = 0;
static uint8_t up_pressed = 0;
static uint8_t down_pressed = 0;

void keys_input_poll(void)
{
    // TODO: mock input for now, replace with actual input handling code later
    left_pressed = 0;
    right_pressed = 0;
    up_pressed = 0;
    down_pressed = 0;
}

uint8_t is_left_pressed(void)
{
    return left_pressed;
}

uint8_t is_right_pressed(void)
{
    return right_pressed;
}

uint8_t is_up_pressed(void)
{
    return up_pressed;
}

uint8_t is_down_pressed(void)
{
    return down_pressed;
}
