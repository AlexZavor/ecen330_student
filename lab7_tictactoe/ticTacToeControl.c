#include "ticTacToeControl.h"
#include "buttons.h"
#include "display.h"
#include "minimax.h"
#include "ticTacToe.h"
#include "ticTacToeDisplay.h"
#include "touchscreen.h"

#include <stdio.h>

#define WAIT_TIME 4.0
#define BTN0 0x01

#define TEXT_SIZE 2
#define LINE1_X 30
#define LINE1_Y (DISPLAY_HEIGHT / 2) - 30
#define LINE2_X 120
#define LINE2_Y (DISPLAY_HEIGHT / 2) - 10
#define LINE3_X 30
#define LINE3_Y (DISPLAY_HEIGHT / 2) + 10
#define LINE4_X 100
#define LINE4_Y (DISPLAY_HEIGHT / 2) + 30

//  ticTacToe Control states
enum ticTacToeControl_st_t {
  MENU,
  FIRST_WAIT,
  COMPUTER_PLAY,
  PLAYER_PLAY,
  PLAYER_WAIT,
  GAME_OVER,
} ticTacToeControl_current_state;

static double tick_time;
static double wait_timer;

static tictactoe_board_t board;
static tictactoe_location_t move;
static bool is_Xs_turn;

// Helper function to draw or erase menu
void drawMenu(bool erase) {
  /*
  Touch board to play X
          -or-
  wait for the computer
      and play O.
  */
  display_setTextColor(erase ? DISPLAY_DARK_BLUE : DISPLAY_WHITE);
  display_setTextSize(TEXT_SIZE);

  // Set the cursor location and print to the LCD
  display_setCursor(LINE1_X, LINE1_Y);
  display_println("Touch board to play X");
  display_setCursor(LINE2_X, LINE2_Y);
  display_println("-or-");
  display_setCursor(LINE3_X, LINE3_Y);
  display_println("wait for the computer");
  display_setCursor(LINE4_X, LINE4_Y);
  display_println("and play O.");
}

// Initialize the tic-tac-toe controller state machine,
// providing the tick period, in seconds.
void ticTacToeControl_init(double period_s) {
  buttons_init();
  display_init();
  ticTacToeControl_current_state = MENU;
  display_fillScreen(DISPLAY_DARK_BLUE);
  drawMenu(false);
  minimax_initBoard(&board);
  is_Xs_turn = true;
  tick_time = period_s;
  wait_timer = 0;
}

// Tick the tic-tac-toe controller state machine
void ticTacToeControl_tick() {
  // Transition
  switch (ticTacToeControl_current_state) {
    // Menu State to print menu and then erase it before game starts
  case MENU:
    // leave if timer finishes
    if (wait_timer >= WAIT_TIME) {
      wait_timer = 0;
      drawMenu(true);
      ticTacToeControl_current_state = FIRST_WAIT;
      ticTacToeDisplay_init();
    }
    break;
    // Waiting state to decide who goes first
  case FIRST_WAIT:
    // computers turn if timer ends
    if (wait_timer >= WAIT_TIME) {
      wait_timer = 0;
      board.squares[0][0] = MINIMAX_X_SQUARE;
      is_Xs_turn = !is_Xs_turn;
      move.column = 0;
      move.row = 0;
      ticTacToeDisplay_drawX(move, 0);
      ticTacToeControl_current_state = PLAYER_WAIT;
      // players turn if screen is touched
    } else if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      touchscreen_ack_touch();
      ticTacToeControl_current_state = PLAYER_PLAY;
    }
    break;
    // Computer's turn state
  case COMPUTER_PLAY:
    ticTacToeControl_current_state = PLAYER_WAIT;
    break;
    // Player's turn state
  case PLAYER_PLAY:
    // check if their move ended the game
    if (minimax_isGameOver(minimax_computeBoardScore(&board, !is_Xs_turn))) {
      ticTacToeControl_current_state = GAME_OVER;
    } else {
      ticTacToeControl_current_state = COMPUTER_PLAY;
    }
    break;
    // waiting for player state
  case PLAYER_WAIT:
    // check if game was won
    if (minimax_isGameOver(minimax_computeBoardScore(&board, !is_Xs_turn))) {
      ticTacToeControl_current_state = GAME_OVER;
      // check if move was valid
    } else if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      touchscreen_ack_touch();
      move = ticTacToeDisplay_getLocationFromPoint(
          touchscreen_get_location()); // if move is valid go to playing state
      if (board.squares[move.row][move.column] == MINIMAX_EMPTY_SQUARE) {
        ticTacToeControl_current_state = PLAYER_PLAY;
      }
    }
    break;
  // Game over State
  case GAME_OVER:
    // restart game if button is pressed
    if (buttons_read() & BTN0 == BTN0) {
      ticTacToeControl_current_state = FIRST_WAIT;
      // erase all played spots
      for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
        for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
          if (board.squares[row][col] == MINIMAX_X_SQUARE) {
            tictactoe_location_t location;
            location.row = row;
            location.column = col;
            ticTacToeDisplay_drawX(location, true);
          } else if (board.squares[row][col] == MINIMAX_O_SQUARE) {
            tictactoe_location_t location;
            location.row = row;
            location.column = col;
            ticTacToeDisplay_drawO(location, true);
          }
        }
      }
      minimax_initBoard(&board);
      is_Xs_turn = true;
      wait_timer = 0;
    }
    break;
    // Error Case
  default:
    printf("TicTacToeControl Transition SM Error\n");
    break;
  }
  // Action
  switch (ticTacToeControl_current_state) {
  case MENU:
    // increment timer
    wait_timer += tick_time;
    break;
  case FIRST_WAIT:
    // increment timer
    wait_timer += tick_time;
    break;
  case COMPUTER_PLAY:
    // calculate and play computer's move with minimax
    move = minimax_computeNextMove(&board, is_Xs_turn);
    board.squares[move.row][move.column] =
        is_Xs_turn ? MINIMAX_X_SQUARE : MINIMAX_O_SQUARE;
    if (is_Xs_turn) {
      ticTacToeDisplay_drawX(move, 0);
    } else {
      ticTacToeDisplay_drawO(move, 0);
    }
    is_Xs_turn = !is_Xs_turn;
    break;
  case PLAYER_PLAY:
    // play players move
    move = ticTacToeDisplay_getLocationFromPoint(touchscreen_get_location());
    board.squares[move.row][move.column] =
        is_Xs_turn ? MINIMAX_X_SQUARE : MINIMAX_O_SQUARE;
    if (is_Xs_turn) {
      ticTacToeDisplay_drawX(move, 0);
    } else {
      ticTacToeDisplay_drawO(move, 0);
    }
    is_Xs_turn = !is_Xs_turn;
    break;
  case PLAYER_WAIT:
    // waiting for player
    break;
  case GAME_OVER:
    // make sure no extra inputs come in
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      touchscreen_ack_touch();
    }
    break;
  default:
    // Error case
    printf("TicTacToeControl Action SM Error\n");
    break;
  }
}
