#include "buttons.h"
#include "display.h"
#include "gameControl.h"
#include "global.h"
#include "interrupts.h"
#include "intervalTimer.h"
#include <stdio.h>

#define TICK_PERIOD 50E-3

// global vars
int score = 0;
int lvl = 0;
int lines = 0;
bool game = true;

// Play board
block board[10][20];
block prevBoard[10][20];
block nextPieceBoard[4][4];

// Interrupt flag method
volatile bool interrupt_flag;
// Interrupt routine
static void isr() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_0);
  interrupt_flag = true;
}

static void drawGameOver() {
  display_fillScreen(DISPLAY_BLACK);

  display_setTextColor(DISPLAY_RED);
  display_setTextSize(4);
  display_setCursor(50, 100);
  display_println("GAME OVER");

  display_setTextColor(DISPLAY_WHITE);
  display_setTextSize(3);
  display_setCursor(80, 180);
  display_print("Score:");
  display_printDecimalInt(score);

  display_setTextSize(1);
  display_setCursor(65, 210);
  display_print("press any button to restart");

  intervalTimer_initCountUp(INTERVAL_TIMER_1);
  intervalTimer_start(INTERVAL_TIMER_1);
  while (intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_1) < 2.0) {
    ;
  }
  intervalTimer_stop(INTERVAL_TIMER_1);
  return;
}

int main() {
gameStart:
  printf("Game Start\n");

  // init variables
  score = 0;
  lvl = 0;
  lines = 0;
  game = true;

  // Draw Menu
  display_init();
  display_fillScreen(DISPLAY_DARK_BLUE);

  display_setTextColor(DISPLAY_WHITE);
  display_setTextSize(2);

  // Set the cursor location and print to the LCD
  display_drawBitmap(80,0,TetrisLogo, 160,120, DISPLAY_RED);
  display_setCursor(13, 140);
  display_println("Left  Down  Rot.  Right");
  display_setCursor(10, 160);
  display_println("  O     O     O     O  ");
  display_setCursor(10, 180);
  display_println("Try and get a High score!");

  // display_drawBitmap(0,0,,360,120,DISPLAY_RED);

  intervalTimer_initCountUp(INTERVAL_TIMER_1);
  intervalTimer_start(INTERVAL_TIMER_1);
  while (intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_1) < 5.0) {
    ;
  }
  intervalTimer_stop(INTERVAL_TIMER_1);

  // Initialize Modules
  gameControl_init(TICK_PERIOD);

  interrupts_init();
  interrupts_irq_enable(INTERVAL_TIMER_0_INTERRUPT_IRQ);
  interrupts_register(INTERVAL_TIMER_0_INTERRUPT_IRQ, isr);
  intervalTimer_initCountDown(INTERVAL_TIMER_0, TICK_PERIOD);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_0);

  intervalTimer_start(INTERVAL_TIMER_0);

  while (1) {
    // Wait for interrupt flag
    while (!interrupt_flag) {
    }
    interrupt_flag = false;

    // Run tick functions
    if (game) {
      gameControl_tick();
    } else {
      drawGameOver();
      intervalTimer_stop(INTERVAL_TIMER_0);
      break;
    }
  }

  uint8_t buttons = buttons_read();
  while (!buttons) {
    // loop endlessly until button is pressed
    buttons = buttons_read();
  }

  goto gameStart;

  return 0;
}
