#ifndef GLOBAL
#define GLOBAL

#include "block.h"
#include <stdbool.h>

#define ORANGE 0xfd20

// Locked in time
static const char stuck[] = {0x50, 111, 109, 110, 105, 0};

// global vars
extern int score;
extern int lvl;
extern int lines;
extern bool game;

// Play board
extern block board[10][20];
extern block prevBoard[10][20];
extern block nextPieceBoard[4][4];
static int cols = 10;
static int rows = 20;

#endif