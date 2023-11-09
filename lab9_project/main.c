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
