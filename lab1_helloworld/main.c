/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

#include <stdio.h>

#include "display.h"

#define TEXT_SIZE 2
#define CURSOR_X 10
#define CURSOR_Y (DISPLAY_HEIGHT / 2)

#define TRIANGLE_DIST_FROM_EDGE 40
#define TRIANGLE_HEIGHT 60
#define TRIANGLE_WIDTH 60

#define CIRCLE_RADIUS 30
#define CIRCLE_DIST_FROM_EDGE 85

// Print out green cross, yellow triangles and red circle to LCD Screen.
int main() {

  // Initialize display driver, and fill scren with black
  display_init();
  display_fillScreen(DISPLAY_BLACK); // Blank the screen.

  // Draw two lines forming green cross
  display_drawLine(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_GREEN);
  display_drawLine(DISPLAY_WIDTH, 0, 0, DISPLAY_HEIGHT, DISPLAY_GREEN);

  // Draw two triangles
  display_fillTriangle(
      DISPLAY_WIDTH / 2, TRIANGLE_DIST_FROM_EDGE + TRIANGLE_HEIGHT,
      (DISPLAY_WIDTH / 2) - (TRIANGLE_WIDTH / 2), TRIANGLE_DIST_FROM_EDGE,
      (DISPLAY_WIDTH / 2) + (TRIANGLE_WIDTH / 2), TRIANGLE_DIST_FROM_EDGE,
      DISPLAY_YELLOW);
  display_drawTriangle(
      DISPLAY_WIDTH / 2,
      DISPLAY_HEIGHT - TRIANGLE_DIST_FROM_EDGE - TRIANGLE_HEIGHT,
      (DISPLAY_WIDTH / 2) - (TRIANGLE_WIDTH / 2),
      DISPLAY_HEIGHT - TRIANGLE_DIST_FROM_EDGE,
      (DISPLAY_WIDTH / 2) + (TRIANGLE_WIDTH / 2),
      DISPLAY_HEIGHT - TRIANGLE_DIST_FROM_EDGE, DISPLAY_YELLOW);

  // Draw two Circles
  display_drawCircle(CIRCLE_DIST_FROM_EDGE, DISPLAY_HEIGHT / 2, CIRCLE_RADIUS,
                     DISPLAY_RED);
  display_fillCircle(DISPLAY_WIDTH - CIRCLE_DIST_FROM_EDGE, DISPLAY_HEIGHT / 2,
                     CIRCLE_RADIUS, DISPLAY_RED);

  return 0;
}
