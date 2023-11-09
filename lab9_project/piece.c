#include "piece.h"
#include "display.h"
#include "global.h"

void piece_init(piece *piece, enum blocktype setShape, int start_x,
                int start_y) {
  piece->shape = setShape;
  piece->x = start_x;
  piece->y = start_y;
  piece->rot = 0;

  // load blocks for the shape
  switch (setShape) {
  case Z:
    piece->blocks[0] = makeVec(0, 0);
    piece->blocks[1] = makeVec(1, 0);
    piece->blocks[2] = makeVec(1, 1);
    piece->blocks[3] = makeVec(2, 1);
    break;
  case S:
    piece->blocks[0] = makeVec(1, 0);
    piece->blocks[1] = makeVec(0, 1);
    piece->blocks[2] = makeVec(1, 1);
    piece->blocks[3] = makeVec(2, 0);
    break;
  case T:
    piece->blocks[0] = makeVec(0, 0);
    piece->blocks[1] = makeVec(1, 0);
    piece->blocks[2] = makeVec(1, 1);
    piece->blocks[3] = makeVec(2, 0);
    break;
  case O:
    piece->blocks[0] = makeVec(0, 0);
    piece->blocks[1] = makeVec(0, 1);
    piece->blocks[2] = makeVec(1, 1);
    piece->blocks[3] = makeVec(1, 0);
    break;
  case I:
    piece->blocks[0] = makeVec(0, 0);
    piece->blocks[1] = makeVec(1, 0);
    piece->blocks[2] = makeVec(2, 0);
    piece->blocks[3] = makeVec(3, 0);
    break;
  case L:
    piece->blocks[0] = makeVec(0, 0);
    piece->blocks[1] = makeVec(1, 0);
    piece->blocks[2] = makeVec(2, 0);
    piece->blocks[3] = makeVec(0, 1);
    break;
  case J:
    piece->blocks[0] = makeVec(0, 0);
    piece->blocks[1] = makeVec(1, 0);
    piece->blocks[2] = makeVec(2, 0);
    piece->blocks[3] = makeVec(2, 1);
    break;
  case X:
    break;
  default:
    break;
  }
}

bool piece_tick(piece *piece, uint8_t add_x, uint8_t add_y) {
  // erase previous shape
  drawPiece(piece, true);
  // collisoin check
  int collide = doescollide(piece, piece->x + add_x, piece->y + add_y);
  if (collide == 2) {
    drawPiece(piece, false);
    piece->shape = X;
    return 0; // return 0 to make it stop updating
  } else if (collide == 1) {
    drawPiece(piece, false);
    return 1;
  } else {
    piece->x += add_x;
    piece->y += add_y;
    // place new shape
    drawPiece(piece, false);
  }
  return 1;
}

void piece_drawPiece(piece *piece, bool erase) {}

// 0 = does not collide | 1 = colides with walls | 2 = collides down
uint8_t doescollide(piece *piece, int check_x, int check_y) {
  for (int i = 0; i < 4; i++) {
    vec2d b = piece->blocks[i];

    if (board[check_x + b.x][check_y + b.y].type != X ||
        check_y + b.y >= rows || check_x + b.x < 0 || check_x + b.x >= cols) {
      return 1 + (check_y > 0);
    }
  }
  return 0;
}
