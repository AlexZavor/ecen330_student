#ifndef GLOBAL
#define GLOBAL

#include "block.h"
#include <stdbool.h>

// Locked in time
static const char stuck[] = {0x50, 111, 109, 110, 105, 0};

// global vars
static int score = 0;
static int lvl = 0;
static int lines = 0;
static bool game = true;

// Play board
static block board[10][20];
static block nextPieceBoard[4][4];
static int cols = 10;
static int rows = 20;

#endif