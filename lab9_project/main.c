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
block nextPieceBoard[4][4];

// Interrupt flag method
volatile bool interrupt_flag;
// Interrupt routine
static void isr() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_0);
  interrupt_flag = true;
}

int main() {

  display_init();
  display_fillScreen(DISPLAY_DARK_BLUE);

  display_setTextColor(DISPLAY_WHITE);
  display_setTextSize(2);

  // Set the cursor location and print to the LCD
  display_setCursor(50, 120);
  display_println("Welcome to Tetris!");
  display_setCursor(13, 140);
  display_println("Left  Down  Rot.  Right");
  display_setCursor(10, 160);
  display_println("  O     O     O     O  ");
  display_setCursor(10, 180);
  display_println("Try and get a High score!");

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
    gameControl_tick();
  }

  return 0;
}
