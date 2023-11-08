#include "touchscreen.h"
#include <stdio.h>

#define SETTLE_TIME 0.05

// Touch screen state machine states
enum touchscreen_st_t {
  WAITING,
  SETTLEING,
  PRESSED,
} touchscreen_current_state;

// touch screen state machine variables
static touchscreen_status_t touchscreen_state;
static double tick_time;
static double settleing_time;

// saved variable for the last touched point
static display_point_t current_display_point;
static uint8_t current_pressure;

// Initialize the touchscreen driver state machine, with a given tick period (in
// seconds).
void touchscreen_init(double period_seconds) {
  touchscreen_state = TOUCHSCREEN_IDLE;
  touchscreen_current_state = WAITING;
  // printf("WAITING\n");
  tick_time = period_seconds;
  settleing_time = 0;
}

// Tick the touchscreen driver state machine
void touchscreen_tick() {
  // transition
  switch (touchscreen_current_state) {
  // waiting for touch state
  case WAITING:
    // go to settleing if display is touched
    if (display_isTouched()) {
      display_clearOldTouchData();
      touchscreen_current_state = SETTLEING;
      // printf("SETTLEING\n");
    }
    break;
  // Waiting for data to settle state
  case SETTLEING:
    // go to waiting if display is released before settled data
    if (!display_isTouched()) {
      touchscreen_current_state = WAITING;
      // printf("WAITING\n");
      // verify press if waited long enough
    } else if (display_isTouched() && (settleing_time >= SETTLE_TIME)) {
      display_getTouchedPoint(&current_display_point.x,
                              &current_display_point.y, &current_pressure);
      touchscreen_current_state = PRESSED;
      // printf("PRESSED\n");
    }
    break;
  // pressed verified, waiting for relase
  case PRESSED:
    if (!display_isTouched()) {
      touchscreen_current_state = WAITING;
      // printf("WAITING\n");
    }
    break;
  // default state for errors
  default:
    // error condition
    printf("TOUCH SCREEN STATE MACHINE BREAK\n");
    break;
  }
  // action
  switch (touchscreen_current_state) {
  // do while waiting. timer is set to 0
  case WAITING:
    settleing_time = 0;
    // sets previous press to released
    if (touchscreen_state == TOUCHSCREEN_PRESSED) {
      touchscreen_state = TOUCHSCREEN_RELEASED;
    }
    break;
  // action while settleing, increase timer by period
  case SETTLEING:
    settleing_time += tick_time;
    break;
  // set state to be pressed when pressed
  case PRESSED:
    touchscreen_state = TOUCHSCREEN_PRESSED;
    break;
  // error state for problems
  default:
    // error condition
    printf("TOUCH SCREEN STATE MACHINE BREAK");
    break;
  }
}

// Return the current status of the touchscreen
touchscreen_status_t touchscreen_get_status() { return touchscreen_state; }

// Acknowledge the touchscreen touch.  This function will only have effect when
// the touchscreen is in the TOUCHSCREEN_RELEASED status, and will cause it to
// switch to the TOUCHSCREEN_IDLE status.
void touchscreen_ack_touch() {
  // check if touch screen is in released state
  // printf("test1\n");
  if (touchscreen_state == TOUCHSCREEN_RELEASED) {
    // printf("test2\n");
    // display_clearOldTouchData();
    // printf("test3\n");
    touchscreen_state = TOUCHSCREEN_IDLE;
  }
}

// Get the (x,y) location of the last touchscreen touch
display_point_t touchscreen_get_location() { return current_display_point; }