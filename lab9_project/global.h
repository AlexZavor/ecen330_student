#ifndef GLOBAL
#define GLOBAL

#include "block.h"
#include <stdbool.h>
#include <stdint.h>

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

// 'Tetris', 160x120px
static uint8_t TetrisLogo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf8, 0xff,
    0xff, 0xc0, 0x07, 0xfe, 0x01, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0xff,
    0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xf0, 0x07, 0xfe,
    0x07, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7,
    0xff, 0xff, 0xf9, 0xff, 0xff, 0xfc, 0x07, 0xfe, 0x0f, 0xff, 0xff, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff,
    0xff, 0xfe, 0x07, 0xfe, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xfe, 0x07, 0xfe,
    0x3f, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7,
    0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x07, 0xfe, 0x3f, 0xff, 0xfe, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff,
    0xff, 0xff, 0x07, 0xfe, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x87, 0xfe,
    0x7f, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7,
    0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x87, 0xfe, 0x7f, 0xff, 0xf8, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff,
    0xff, 0xff, 0x87, 0xfe, 0x7f, 0xff, 0xf9, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x87, 0xfe,
    0x7f, 0xff, 0xf0, 0x50, 0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7,
    0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xf9, 0xff,
    0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x7f, 0xff, 0xff,
    0x1f, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x87, 0xfe,
    0x7f, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x87, 0xff, 0x87, 0xfe, 0x7f, 0xf8, 0x00, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x87, 0xff, 0x87, 0xfe, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x87, 0xff, 0x87, 0xfe,
    0x7f, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x8f, 0xff, 0x87, 0xfe, 0x7f, 0xf8, 0x00, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x8f, 0xff, 0x07, 0xfe, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xff, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x9f, 0xff, 0x07, 0xfe,
    0x3f, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xff, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x9f, 0xfe, 0x07, 0xfe, 0x1f, 0xfe, 0x00, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xfe, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0xff, 0xfe, 0x07, 0xfe, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xfe, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0xff, 0xfc, 0x07, 0xfe,
    0x0f, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xfc, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0xff, 0xf8, 0x07, 0xfe, 0x0f, 0xff, 0x80, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xfc, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0xff, 0xf8, 0x07, 0xfe, 0x07, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xf8, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0xff, 0xfc, 0x07, 0xfe,
    0x07, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xf8, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0xbf, 0xfc, 0x07, 0xfe, 0x03, 0xff, 0xc0, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xf8, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x9f, 0xfe, 0x07, 0xfe, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xf0, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x9f, 0xfe, 0x07, 0xfe,
    0x01, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xf0, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x8f, 0xfe, 0x07, 0xfe, 0x00, 0xff, 0xf0, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xe0, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x8f, 0xff, 0x07, 0xfe, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xe0, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x87, 0xff, 0x07, 0xfe,
    0x00, 0x7f, 0xf8, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x87, 0xff, 0x87, 0xfe, 0x00, 0x7f, 0xfc, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x83, 0xff, 0x87, 0xfe, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x83, 0xff, 0xc7, 0xfe,
    0x00, 0x1f, 0xfe, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x81, 0xff, 0xc7, 0xfe, 0x00, 0x1f, 0xfe, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x81, 0xff, 0xc7, 0xfe, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x81, 0xff, 0xc7, 0xfe,
    0x00, 0x0f, 0xff, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00,
    0x0f, 0xfc, 0x01, 0xff, 0x80, 0xff, 0xc7, 0xfe, 0x00, 0x07, 0xff, 0x80,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff,
    0x80, 0xff, 0xc7, 0xfe, 0x00, 0x07, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x01, 0xff, 0x80, 0x7f, 0xc7, 0xfe,
    0x00, 0x1f, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xf0,
    0x0f, 0xfc, 0x01, 0xff, 0x80, 0x7f, 0xc7, 0xfe, 0x7f, 0xff, 0xff, 0x80,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xfc, 0x01, 0xff,
    0x80, 0x3f, 0xc7, 0xfe, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xfc, 0x01, 0xff, 0x80, 0x3f, 0xc7, 0xfe,
    0x7f, 0xff, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xfc,
    0x0f, 0xfc, 0x01, 0xff, 0x80, 0x1f, 0xc7, 0xfe, 0x7f, 0xff, 0xff, 0x80,
    0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xfc, 0x0f, 0xfc, 0x01, 0xff,
    0x80, 0x1f, 0xc7, 0xfe, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x1f, 0xff, 0xff, 0xfe, 0x0f, 0xfc, 0x01, 0xff, 0x80, 0x1f, 0xc7, 0xfe,
    0x7f, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xfe,
    0x0f, 0xfc, 0x01, 0xff, 0x80, 0x0f, 0xc7, 0xfe, 0x7f, 0xff, 0xff, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xfe, 0x0f, 0xfc, 0x01, 0xff,
    0x80, 0x0f, 0xc7, 0xfe, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x07, 0xff, 0xff, 0xff, 0x0f, 0xfc, 0x01, 0xff, 0x80, 0x07, 0xc7, 0xfe,
    0x7f, 0xff, 0xfe, 0x00, 0x00, 0x01, 0xff, 0x80, 0x07, 0xff, 0xff, 0xff,
    0x0f, 0xfc, 0x01, 0xff, 0x80, 0x07, 0xc7, 0xfe, 0x7f, 0xff, 0xfc, 0x00,
    0x00, 0x01, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0x8f, 0xfc, 0x01, 0xff,
    0x80, 0x03, 0xc7, 0xfe, 0x7f, 0xff, 0xf8, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x01, 0xff, 0xff, 0xff, 0x8f, 0xfc, 0x01, 0xff, 0x80, 0x03, 0xc7, 0xfe,
    0x7f, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff,
    0xcf, 0xfc, 0x01, 0xff, 0x80, 0x01, 0xc7, 0xfe, 0x7f, 0xff, 0xc0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#endif