#ifndef PIECE
#define PIECE

#include "block.h"
#include "vec2d.h"
#include <stdbool.h>
#include <stdint.h>

// piece moving
typedef struct {
  enum blocktype shape;
  vec2d blocks[4];
  uint8_t x;
  uint8_t y;
  uint8_t rot;
} piece;

void piece_init(piece *piece, enum blocktype setShape, int start_x,
                int start_y);

bool piece_tick(piece *piece, int8_t add_x, int8_t add_y);

void piece_drawPiece(piece *piece, bool erase);

uint8_t piece_doescollide(piece *piece, int8_t check_x, int8_t check_y);

uint8_t piece_rotate(piece *piece, int8_t rotation);

#endif