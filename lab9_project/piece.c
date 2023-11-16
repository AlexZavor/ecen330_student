#include "piece.h"
#include "display.h"
#include "global.h"
#include <stdio.h>

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

bool piece_tick(piece *piece, int8_t add_x, int8_t add_y) {
  // erase previous shape
  piece_drawPiece(piece, true);
  // collisoin check
  int collide = piece_doescollide(piece, piece->x + add_x, piece->y + add_y);
  if (collide == 2) {
    piece_drawPiece(piece, false);
    piece->shape = X;
    return 0; // return 0 to make it stop updating
  } else if (collide == 1) {
    // Updated into a wall. Just don't do anything
    piece_drawPiece(piece, false);
    return 1;
  } else if (collide == 0) {
    piece->x += add_x;
    piece->y += add_y;
    // place new shape
    piece_drawPiece(piece, false);
  } else {
    printf("Error\n");
  }
  return 1;
}

void piece_drawPiece(piece *piece, bool erase) {
  enum blocktype shape = erase ? X : piece->shape;
  for (uint8_t i = 0; i < 4; i++) {
    vec2d b = piece->blocks[i];
    board[piece->x + b.x][piece->y + b.y].type = shape;
  }
}

// 0 = does not collide | 1 = colides with walls | 2 = collides down
uint8_t piece_doescollide(piece *piece, int8_t check_x, int8_t check_y) {
  for (uint8_t i = 0; i < 4; i++) {
    vec2d b = piece->blocks[i];

    if (board[check_x + b.x][check_y + b.y].type != X ||
        check_y + b.y >= rows || check_x + b.x < 0 || check_x + b.x >= cols) {
      return 1 + (int)(check_y > piece->y);
    }
  }
  return 0;
}

uint8_t piece_rotate(piece *piece, int8_t rotation) {
  // erase previous shape
  piece_drawPiece(piece, true);

  // scroll
  if (rotation > 0) {
    if (piece->rot < 3) {
      piece->rot += 1;
    } else {
      piece->rot = 0;
    }
  } else if (rotation < 0) {
    if (piece->rot > 0) {
      piece->rot -= 1;
    } else {
      piece->rot = 3;
    }
  }

  // load different shape for each block
  switch (piece->shape) {
  case Z:
    if (piece->rot % 2) {
      piece->blocks[0] = makeVec(1, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(0, 2);
    } else {
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(2, 1);
    }
    break;
  case S:
    if (piece->rot % 2) {
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(1, 2);
    } else {
      piece->blocks[0] = makeVec(1, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(2, 0);
    }
    break;
  case T:
    switch (piece->rot) {
    case 0:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(2, 0);
      break;
    case 1:
      piece->blocks[0] = makeVec(1, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(1, 2);
      break;
    case 2:
      piece->blocks[0] = makeVec(1, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(2, 1);
      break;
    case 3:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(0, 2);
      break;
    default:
      break;
    }
    break;
  case O:
    break;
  case I:
    if (piece->rot % 2) {
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(0, 2);
      piece->blocks[3] = makeVec(0, 3);
    } else {
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(2, 0);
      piece->blocks[3] = makeVec(3, 0);
    }
    break;
  case L:
    switch (piece->rot) {
    case 0:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(2, 0);
      piece->blocks[3] = makeVec(0, 1);
      break;
    case 1:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(1, 2);
      break;
    case 2:
      piece->blocks[0] = makeVec(0, 1);
      piece->blocks[1] = makeVec(1, 1);
      piece->blocks[2] = makeVec(2, 1);
      piece->blocks[3] = makeVec(2, 0);
      break;
    case 3:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(0, 2);
      piece->blocks[3] = makeVec(1, 2);
      break;
    default:
      break;
    }
    break;
  case J:
    switch (piece->rot) {
    case 0:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(2, 0);
      piece->blocks[3] = makeVec(2, 1);
      break;
    case 1:
      piece->blocks[0] = makeVec(1, 0);
      piece->blocks[1] = makeVec(1, 1);
      piece->blocks[2] = makeVec(1, 2);
      piece->blocks[3] = makeVec(0, 2);
      break;
    case 2:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(0, 1);
      piece->blocks[2] = makeVec(1, 1);
      piece->blocks[3] = makeVec(2, 1);
      break;
    case 3:
      piece->blocks[0] = makeVec(0, 0);
      piece->blocks[1] = makeVec(1, 0);
      piece->blocks[2] = makeVec(0, 1);
      piece->blocks[3] = makeVec(0, 2);
      break;
    default:
      break;
    }
    break;
  case X:
    break;
  default:
    break;
  }

  if (piece_doescollide(piece, piece->x, piece->y)) {
    // go back
    piece_rotate(piece, -1);
  } else {
    // place new shape
    piece_drawPiece(piece, false);
  }
  return 0;
}
