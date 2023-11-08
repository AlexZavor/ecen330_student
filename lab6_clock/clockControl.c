#include "clockControl.h"
#include "clockDisplay.h"
#include "touchscreen.h"
#include <stdio.h>

#define LONG_HOLD_TIME 0.5
#define FAST_UPDATE_PERIOD 0.1

//  clock control states
enum clockControl_st_t {
  WAITING,
  INC_DEC,
  LONG_PRESS_DELAY,
  FAST_UPDATE,
} clockcontrol_current_state;

static double tick_time;
static double press_time;
static double fast_update_time;

// All printed messages for states are provided here.
#define WAITING_ST_MSG "WAITING\n"
#define INC_DEC_ST_MSG "INC DEC\n"
#define LONG_PRESS_DELAY_ST_MSG "LONG PRESS DELAY\n"
#define FAST_UPDATE_ST_MSG "FAST UPDATE\n"
// This is a debug state print routine. It will print the names of the states
// each time tick() is called. It only prints states if they are different than
// the previous state.
void debugStatePrint() {
  static enum clockControl_st_t previousState;
  static char firstPass = 1;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state
  // name over and over.
  if (previousState != clockcontrol_current_state || firstPass) {
    firstPass = 0; // previousState will be defined, firstPass is false.
    // remember what previous state was
    previousState = clockcontrol_current_state;
    // print out the matching message to the current state.
    switch (clockcontrol_current_state) {
    case WAITING:
      printf(WAITING_ST_MSG);
      break;
    case INC_DEC:
      printf(INC_DEC_ST_MSG);
      break;
    case LONG_PRESS_DELAY:
      printf(LONG_PRESS_DELAY_ST_MSG);
      break;
    case FAST_UPDATE:
      printf(FAST_UPDATE_ST_MSG);
      break;
    }
  }
}

// Initialize the clock control state machine, with a given period in seconds.
void clockControl_init(double period_s) {
  clockcontrol_current_state = WAITING;
  tick_time = period_s;
  press_time = 0;
  fast_update_time = 0;
}

// Tick the clock control state machine
void clockControl_tick() {
  debugStatePrint();
  // Transition
  switch (clockcontrol_current_state) {
  case WAITING:
    // while waiting look for the touchscreen being pressed or a missed press
    if (touchscreen_get_status() == TOUCHSCREEN_PRESSED) {
      press_time = 0;
      clockcontrol_current_state = LONG_PRESS_DELAY;
    } else if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      clockcontrol_current_state = INC_DEC;
    }
    break;
  case INC_DEC:
    // wait for the touchscreen to be released and acknoledge the press.
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      clockcontrol_current_state = WAITING;
      touchscreen_ack_touch();
    }
    break;
  case LONG_PRESS_DELAY:
    // wait and start to count the timer to see if it is a long press.
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      clockcontrol_current_state = INC_DEC;
    } else if (press_time >= LONG_HOLD_TIME) {
      clockcontrol_current_state = FAST_UPDATE;
      fast_update_time = 0;
    }
    break;
  case FAST_UPDATE:
    // wait for touchscreen to be released and update every 0.1 seconds.
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      clockcontrol_current_state = WAITING;
      touchscreen_ack_touch();
    } else if (fast_update_time >= FAST_UPDATE_PERIOD) {
      fast_update_time = 0;
      clockDisplay_performIncDec(touchscreen_get_location());
    }
    break;
  default:
    // error condition
    printf("CLOCK CONTROL STATE MACHINE BREAK");
    break;
  }
  // Action
  switch (clockcontrol_current_state) {
  case WAITING:
    // no action while waiting
    break;
  case INC_DEC:
    // increment the display based on where it was pressed
    clockDisplay_performIncDec(touchscreen_get_location());
    break;
  case LONG_PRESS_DELAY:
    // increment long press timer
    press_time += tick_time;
    break;
  case FAST_UPDATE:
    // increment fast update timer
    fast_update_time += tick_time;
    break;
  default:
    // error condition
    printf("CLOCK CONTROL STATE MACHINE BREAK");
    break;
  }
}