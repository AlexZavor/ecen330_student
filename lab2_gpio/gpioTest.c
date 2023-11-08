#include "gpioTest.h"
#include "buttons.h"
#include "display.h"
#include "leds.h"
#include "switches.h"

#define TEXT_SIZE 2
#define CURSOR_Y (DISPLAY_HEIGHT / 4)
#define CURSOR_X_OFFSET 20

void gpioTest_buttons() {
  // set buttons to output
  buttons_init();
  // initialize the display
  display_init();
  // Blank the screen.
  display_fillScreen(DISPLAY_BLACK);
  display_setTextSize(TEXT_SIZE);

  // Read buttons and save their previous state
  uint8_t buttons = buttons_read();
  uint8_t buttonsState = buttons;

  // loops through checking each button until all are pressed simulatniously
  while (buttons_read() != 0x0F) {
    buttons = buttons_read();
    // Tests the current and previous state of button three and run if there is
    // a change in state
    if ((buttons & BUTTONS_BTN3_MASK) != (buttonsState & BUTTONS_BTN3_MASK)) {
      // if the change is positive then the button is pressed, otherwise
      // released, draw the required box or clear the area by drawing a black
      // box
      if (buttons & BUTTONS_BTN3_MASK) {
        display_fillRect(0, 0, DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 2,
                         DISPLAY_BLUE);
        display_setTextColor(DISPLAY_WHITE);
        display_setCursor(CURSOR_X_OFFSET, CURSOR_Y);
        display_print("BTN3");
      } else {
        display_fillRect(0, 0, DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 2,
                         DISPLAY_BLACK);
      }
    }
    // BTN2
    if ((buttons & BUTTONS_BTN2_MASK) != (buttonsState & BUTTONS_BTN2_MASK)) {
      // if the change is positive then the button is pressed, otherwise
      // released, draw the required box or clear the area by drawing a black
      // box
      if (buttons & BUTTONS_BTN2_MASK) {
        display_fillRect(DISPLAY_WIDTH / 4, 0, DISPLAY_WIDTH / 4,
                         DISPLAY_HEIGHT / 2, DISPLAY_RED);
        display_setTextColor(DISPLAY_WHITE);
        display_setCursor(DISPLAY_WIDTH / 4 + CURSOR_X_OFFSET, CURSOR_Y);
        display_print("BTN2");
      } else {
        display_fillRect(DISPLAY_WIDTH / 4, 0, DISPLAY_WIDTH / 4,
                         DISPLAY_HEIGHT / 2, DISPLAY_BLACK);
      }
    }
    // BTN1
    if ((buttons & BUTTONS_BTN1_MASK) != (buttonsState & BUTTONS_BTN1_MASK)) {
      // if the change is positive then the button is pressed, otherwise
      // released, draw the required box or clear the area by drawing a black
      // box
      if (buttons & BUTTONS_BTN1_MASK) {
        display_fillRect(DISPLAY_WIDTH / 2, 0, DISPLAY_WIDTH / 4,
                         DISPLAY_HEIGHT / 2, DISPLAY_GREEN);
        display_setTextColor(DISPLAY_BLACK);
        display_setCursor((DISPLAY_WIDTH / 2) + CURSOR_X_OFFSET, CURSOR_Y);
        display_print("BTN1");
      } else {
        display_fillRect(DISPLAY_WIDTH / 2, 0, DISPLAY_WIDTH / 4,
                         DISPLAY_HEIGHT / 2, DISPLAY_BLACK);
      }
    }
    // BTN0
    if ((buttons & BUTTONS_BTN0_MASK) != (buttonsState & BUTTONS_BTN0_MASK)) {
      // if the change is positive then the button is pressed, otherwise
      // released, draw the required box or clear the area by drawing a black
      // box
      if (buttons & BUTTONS_BTN0_MASK) {
        display_fillRect(3 * (DISPLAY_WIDTH / 4), 0, DISPLAY_WIDTH / 4,
                         DISPLAY_HEIGHT / 2, DISPLAY_YELLOW);
        display_setTextColor(DISPLAY_BLACK);
        display_setCursor(3 * (DISPLAY_WIDTH / 4) + CURSOR_X_OFFSET, CURSOR_Y);
        display_print("BTN0");
      } else {
        display_fillRect(03 * (DISPLAY_WIDTH / 4), 0, DISPLAY_WIDTH / 4,
                         DISPLAY_HEIGHT / 2, DISPLAY_BLACK);
      }
    }
    // setting the state to be the previous state of buttons
    buttonsState = buttons;
  }
  display_fillScreen(DISPLAY_BLACK); // Blank the screen.
}

void gpioTest_switches() {
  // initialize switches to be inputs
  switches_init();
  // initialize LEDS
  leds_init();

  // check the position of the switches. exit if all switches are up.
  while (switches_read() != 0x0F) {
    // illuminate LED corisponding to a switch if the switch is on.
    leds_write(switches_read());
  }
  leds_write(0x00);
}