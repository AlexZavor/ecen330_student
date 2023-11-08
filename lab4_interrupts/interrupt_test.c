#include "interrupt_test.h"
#include "interrupts.h"
#include "intervalTimer.h"
#include "leds.h"

#include <stdio.h>

#define ONE_TENTH_HZ_PERIOD 10
#define ONE_HZ_PERIOD 1
#define TEN_HZ_PERIOD 0.1

#define LED0 1
#define LED1 2
#define LED2 4

// Interrupt sub routine for timer 0 which should be called once every tenth of
// a seccond. flips LED 0.
void isr_timer0() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_0_INTERRUPT_IRQ);
  leds_write(leds_read() ^ LED0);
}
// Interrupt sub routine for timer 1 which should be called once every seccond.
// flips LED 1.
void isr_timer1() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_1_INTERRUPT_IRQ);
  leds_write(leds_read() ^ LED1);
}
// Interrupt sub routine for timer 2 which should be called once every ten
// secconds. flips LED 2.
void isr_timer2() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_2_INTERRUPT_IRQ);
  leds_write(leds_read() ^ LED2);
}

// blink LEDs at intervals of 10hz, 1hz and 0.1hz with timers and interrupts
void interrupt_test_run() {
  // initalize leds and interrupts.
  leds_init();
  interrupts_init();

  // initiallize Interval Timer 0 to have a ten hertz interrupt signal.
  // register the function isr_timer0 to the interrupt
  intervalTimer_initCountDown(INTERVAL_TIMER_0, TEN_HZ_PERIOD);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_0);
  interrupts_register(INTERVAL_TIMER_0_INTERRUPT_IRQ, isr_timer0);
  interrupts_irq_enable(INTERVAL_TIMER_0_INTERRUPT_IRQ);

  // initiallize Interval Timer 1 to have a one hertz interrupt signal.
  // register the function isr_timer1 to the interrupt
  intervalTimer_initCountDown(INTERVAL_TIMER_1, ONE_HZ_PERIOD);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_1);
  interrupts_register(INTERVAL_TIMER_1_INTERRUPT_IRQ, isr_timer1);
  interrupts_irq_enable(INTERVAL_TIMER_1_INTERRUPT_IRQ);

  // initiallize Interval Timer 2 to have a one tenth hertz interrupt signal.
  // register the function isr_timer2 to the interrupt
  intervalTimer_initCountDown(INTERVAL_TIMER_2, ONE_TENTH_HZ_PERIOD);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_2);
  interrupts_irq_enable(INTERVAL_TIMER_2_INTERRUPT_IRQ);
  interrupts_register(INTERVAL_TIMER_2_INTERRUPT_IRQ, isr_timer2);

  // Start all three timers.
  intervalTimer_start(INTERVAL_TIMER_0);
  intervalTimer_start(INTERVAL_TIMER_1);
  intervalTimer_start(INTERVAL_TIMER_2);

  // loop to keep the CPU active while waiting for interrupts.
  while (1)
    ;
}
