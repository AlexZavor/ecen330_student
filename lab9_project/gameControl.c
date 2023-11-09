#include "buttons.h"
#include "display.h"
#include "global.h"
#include "piece.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Pieces
static piece currentPiece;
static piece nextPiece;

static double tick_time;
static double piece_tick_time;
static double piece_tick_timer;

// Helper function checking through each row to see if it has been cleared and
// fixes the board to refelct changes Returns the number of rows cleared
static uint8_t checkRowClear() {
  int8_t cleared[4] = {-1, -1, -1, -1};
  int8_t rowsclear = 0;
  for (int8_t y = rows; y > 0; y--) // check though each row
  {
    bool clear = true;
    for (int8_t x = 0; x < cols; x++) // check each block
    {
      if (board[x][y].type == X) {
        clear = false;
      }
    }
    if (clear) {
      cleared[rowsclear] = y;
      rowsclear++;
    }
  }
  // add points
  switch (rowsclear) {
  case 1:
    score += 40 * (lvl + 1);
    break;
  case 2:
    score += 100 * (lvl + 1);
    break;
  case 3:
    score += 300 * (lvl + 1);
    break;
  case 4:
    score += 1200 * (lvl + 1);
    break;
  default:
    break;
  }

  // remove rows and bring rest down
  for (int8_t i = 3; i >= 0; i--) {
    if (cleared[i] != -1) { // for each non-null row to remove
      for (int8_t y_off = 0; y_off + cleared[i] != 0;
           y_off--) // go though each row above it
      {
        for (int8_t x = 0; x < cols; x++) {
          if (cleared[i] + y_off == 0) {
            board[x][cleared[i] + y_off].type = X;
          } else {
            board[x][cleared[i] + y_off].type =
                board[x][cleared[i] + y_off - 1].type;
          }
        }
      }
    }
  }

  return rowsclear;
}

// Helper function that transitions the next piece to the new piece and creates
// a new next piece
static void setNewPiece() {
  for (uint8_t i = 0; i < 4; i++) {
    currentPiece.blocks[i] = nextPiece.blocks[i];
  }
  currentPiece.rot = nextPiece.rot;
  currentPiece.shape = nextPiece.shape;
  currentPiece.x = nextPiece.x;
  currentPiece.y = nextPiece.y;
  // make next piece
  piece_init(&nextPiece, rand() % 7, 4, 0);
  // clear next piece board
  // for (uint8_t x = 0; x < 4; x++) {
  //   for (uint8_t y = 0; y < 4; y++) {
  //     nextPieceBoard[x][y].type = X;
  //   }
  // }
  // // draw next piece
  // for (uint8_t i = 0; i < 4; i++) {
  //   vec2d b = nextPiece.blocks[i];
  //   nextPieceBoard[b.x][b.y].type = nextPiece.shape;
  // }
  // check for a game over
  if (doescollide(&currentPiece, 4, 0)) {
    game = false;
    for (uint8_t x = 0; x < cols; x++) {
      for (uint8_t y = 0; y < rows; y++) {
        board[x][y].type = X;
      }
    }
    printf("game ended\n");
  } else {
    // draw new piece
    printf("new Piece\n");
    piece_drawPiece(&currentPiece, false);
  }
}

void gameControl_init(double period_s) {
  // initialize buttons
  buttons_init();
  // Clear screen
  display_init();
  display_fillScreen(DISPLAY_BLACK);

  // set board to empty
  for (int col = 0; col < cols; col++) {
    for (int row = 0; row < rows; row++) {
      board[col][row].type = X;
    }
  }


  // std::srand(int(std::time(nullptr)));
  piece_init(&nextPiece, rand() % 7, 4, 0);
  piece_init(&currentPiece, X, 4, 0);

  tick_time = period_s;
  piece_tick_time = 1 - (lvl * .05);
  piece_tick_timer = 0;
}

void gameControl_tick() {

  // Check for inputs
  if (buttons_read()) {
    printf("HIT\n");
  }

  // wait for piece tick timer
  piece_tick_timer += tick_time;
  if (piece_tick_timer >= piece_tick_time) {
    // Check that we have a current piece, make piece, else tick current piece
    piece_tick_timer = 0;

    // first round logic
    if (currentPiece.shape == X) {
      // Give us a new current piece
      setNewPiece();
    } else {
      // tick the current piece
      if (!piece_tick(&currentPiece, 0, 1)) {
        // if it fails to update then make new current piece
        setNewPiece();
      }
    }
  }

  // Check for cleared rows
  lines += checkRowClear();

  display_drawRect(0, 0, 84, 164, DISPLAY_MAGENTA);

  // Draw board
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      block currentblock = board[x][y];
      // if (currentblock.type != X) {
      uint16_t color;

      switch (currentblock.type) {
      case Z:
        color = DISPLAY_RED;
        break;
      case S:
        color = DISPLAY_GREEN;
        break;
      case T:
        color = DISPLAY_MAGENTA;
        break;
      case O:
        color = DISPLAY_YELLOW;
        break;
      case I:
        color = DISPLAY_CYAN;
        break;
      case L:
        color = DISPLAY_DARK_YELLOW;
        break;
      case J:
        color = DISPLAY_DARK_BLUE;
        break;
      case X:
        color = DISPLAY_BLACK;
        break;
      default:
        printf("somethingwrong\n");
        break;
      }

      display_drawRect((x * 8 + 2), (y * 8 + 2), 7, 7, color);
      // display_drawRect((x * 8 + 3), (y * 8 + 3), 5, 5, color);
      // display_drawRect((x * 8 + 4), (y * 8 + 4), 3, 3, color);
      // }
    }
  }
  // Draw accessorys

  // Update Data
  lvl = lines / 10;
  piece_tick_time = 1 - (lvl * .05);
}
