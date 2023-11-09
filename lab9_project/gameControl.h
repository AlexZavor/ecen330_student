#ifndef GAMECONTROL
#define GAMECONTROL

#include <stdbool.h>

// Initalize the Tetris game controller
void gameControl_init(double period_s);

// Tick the tetris game controller
void gameControl_tick();

#endif /* GAMECONTROL */