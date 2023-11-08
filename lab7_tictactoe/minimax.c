#include "minimax.h"

#define TOP 0
#define MID 1
#define BOT 2
#define LFT 0
#define RGT 2

// Recurseive minimax function. returns score at bottom of most likely tree.
minimax_score_t minimax(tictactoe_board_t *board, bool is_Xs_turn,
                        uint8_t depth) {
  minimax_score_t currentScore = minimax_computeBoardScore(board, !is_Xs_turn);
  depth++;

  // if minimax tree reaches a finished game return with the depth
  if (minimax_isGameOver(currentScore)) {
    return currentScore / depth;
  }

  // set score to worst possible option
  minimax_score_t score;
  if (is_Xs_turn) {
    score = MINIMAX_O_WINNING_SCORE;
  } else {
    score = MINIMAX_X_WINNING_SCORE;
  }

  // minimax recursively for every empty square
  for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
    for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
      if (board->squares[row][col] == MINIMAX_EMPTY_SQUARE) {

        // Simulate playing at this location
        board->squares[row][col] =
            is_Xs_turn ? MINIMAX_X_SQUARE : MINIMAX_O_SQUARE;

        // Recursively call minimax to get the best score, assuming player
        // choses to play at this location.
        minimax_score_t boardScore;
        boardScore = minimax(board, !is_Xs_turn, depth);
        if (is_Xs_turn) {
          if (boardScore > score) {
            score = boardScore;
          }
        } else {
          if (boardScore < score) {
            score = boardScore;
          }
        }

        // Undo the change to the board
        board->squares[row][col] = MINIMAX_EMPTY_SQUARE;
      }
    }
  }

  return score;
}

// This routine is not recursive but will invoke the recursive minimax function.
// You will call this function from the controlling state machine that you will
// implement in a later milestone. It computes the row and column of the next
// move based upon: the current board and player.
tictactoe_location_t minimax_computeNextMove(tictactoe_board_t *board,
                                             bool is_Xs_turn) {
  tictactoe_location_t move;

  // Create a 2d array of scores to keep track of each possibility
  minimax_score_t scoreTable[TICTACTOE_BOARD_ROWS][TICTACTOE_BOARD_COLUMNS];

  // call minimax recursively for every empty square
  for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
    for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
      if (board->squares[row][col] == MINIMAX_EMPTY_SQUARE) {

        // Simulate playing at this location
        board->squares[row][col] =
            is_Xs_turn ? MINIMAX_X_SQUARE : MINIMAX_O_SQUARE;

        // Recursively call minimax to get the best score, assuming player
        // choses to play at this location.
        // fill in table of scores
        scoreTable[row][col] = minimax(board, !is_Xs_turn, 0);

        // Undo the change to the board
        board->squares[row][col] = MINIMAX_EMPTY_SQUARE;
      } else
        // Make sure to fill all spaces of the board
        scoreTable[row][col] = MINIMAX_NOT_ENDGAME;
    }
  }

  // Set the score to the worst possible case
  minimax_score_t score;
  if (is_Xs_turn) {
    score = MINIMAX_O_WINNING_SCORE;
  } else {
    score = MINIMAX_X_WINNING_SCORE;
  }
  // iterate through all of the possible options and select the best based on
  // the score
  for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
    for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
      // select the best or worst option
      if (is_Xs_turn) {
        if (scoreTable[row][col] != MINIMAX_NOT_ENDGAME &&
            scoreTable[row][col] > score) {
          score = scoreTable[row][col];
          move.row = row;
          move.column = col;
        }
      } else {
        if (scoreTable[row][col] != MINIMAX_NOT_ENDGAME &&
            scoreTable[row][col] < score) {
          score = scoreTable[row][col];
          move.row = row;
          move.column = col;
        }
      }
    }
  }
  return move;
}

// Returns the score of the board.
// This returns one of 4 values: MINIMAX_X_WINNING_SCORE,
// MINIMAX_O_WINNING_SCORE, MINIMAX_DRAW_SCORE, MINIMAX_NOT_ENDGAME
// Note: the is_Xs_turn argument indicates which player just took their
// turn and makes it possible to speed up this function.
// Assumptions:
// (1) if is_Xs_turn == true, the last thing played was an 'X'.
// (2) if is_Xs_turn == false, the last thing played was an 'O'.
// Hint: If you know the game was not over when an 'X' was played,
// you don't need to look for 'O's, and vice-versa.
minimax_score_t minimax_computeBoardScore(tictactoe_board_t *board,
                                          bool is_Xs_turn) {
  tictactoe_square_state_t lookingFor;
  minimax_score_t possibleWin;
  // set what function is looking for based on who's turn it is.
  if (is_Xs_turn) {
    lookingFor = MINIMAX_X_SQUARE;
    possibleWin = MINIMAX_X_WINNING_SCORE;
  } else {
    lookingFor = MINIMAX_O_SQUARE;
    possibleWin = MINIMAX_O_WINNING_SCORE;
  }

  // rows
  for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
    if (board->squares[row][LFT] == lookingFor &&
        board->squares[row][MID] == lookingFor &&
        board->squares[row][RGT] == lookingFor) {
      return possibleWin;
    }
  }
  // cols
  for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
    if (board->squares[TOP][col] == lookingFor &&
        board->squares[MID][col] == lookingFor &&
        board->squares[BOT][col] == lookingFor) {
      return possibleWin;
    }
  }
  // diag
  if (board->squares[TOP][LFT] == lookingFor &&
      board->squares[MID][MID] == lookingFor &&
      board->squares[BOT][RGT] == lookingFor) {
    return possibleWin;
  }
  if (board->squares[TOP][RGT] == lookingFor &&
      board->squares[MID][MID] == lookingFor &&
      board->squares[BOT][LFT] == lookingFor) {
    return possibleWin;
  }

  // check for empty squares
  for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
    for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
      if (board->squares[row][col] == MINIMAX_EMPTY_SQUARE) {
        return MINIMAX_NOT_ENDGAME;
      }
    }
  }
  return MINIMAX_DRAW_SCORE;
}

// Init the board to all empty squares.
void minimax_initBoard(tictactoe_board_t *board) {
  // loop through all points and set them to empty
  for (uint8_t row = 0; row < TICTACTOE_BOARD_ROWS; row++) {
    for (uint8_t col = 0; col < TICTACTOE_BOARD_COLUMNS; col++) {
      board->squares[row][col] = MINIMAX_EMPTY_SQUARE;
    }
  }
}

// Determine that the game is over by looking at the score.
bool minimax_isGameOver(minimax_score_t score) {
  // check if score is endgame (X wins, O wins, or Draw)
  if (score == MINIMAX_NOT_ENDGAME) {
    return 0;
  }
  return 1;
}