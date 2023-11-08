#include "plane.h"
#include "config.h"
#include "display.h"
#include "missile.h"
#include <stdio.h>

#define FLY_HEIGHT 50
#define SPAWN_TICKS_MAX 60

#define PLANE_WIDTH 40
#define PLANE_HEIGHT 25
#define HALF 2

enum plane_st_t {
  WAIT,
  FLYING,
  DEAD,
} planeCurrentState;

static missile_t *missile;

static int16_t x_pos;
static int16_t y_pos;

static int16_t waitTime;
static int16_t missileDropPoint;
static bool hasLaunched;

// Initialize the plane state machine
// Pass in a pointer to the missile struct (the plane will only have one
// missile)
void plane_init(missile_t *plane_missile) {
  missile = plane_missile;
  // Decide Random Wait time
  waitTime = rand() % SPAWN_TICKS_MAX;
  // decide random drop point
  missileDropPoint = rand() % DISPLAY_WIDTH;
  hasLaunched = false;

  // set x and y
  x_pos = DISPLAY_WIDTH;
  y_pos = FLY_HEIGHT;
  // set state to wait
  planeCurrentState = WAIT;
}

// State machine tick function
void plane_tick() {
  // Transition
  switch (planeCurrentState) {
  case WAIT:
    if (waitTime <= 0) {
      planeCurrentState = FLYING;
    }
    break;
  case FLYING:
    if (x_pos <= -PLANE_WIDTH) {
      planeCurrentState = DEAD;
    }
    break;
  case DEAD:
    planeCurrentState = WAIT;
    break;
  default:
    printf("Plane State machine transition error\n");
    break;
  }
  // Action
  switch (planeCurrentState) {
  case WAIT:
    waitTime--;
    break;
  case FLYING:
    display_fillTriangle(x_pos, y_pos + (PLANE_HEIGHT / HALF),
                         x_pos + PLANE_WIDTH, FLY_HEIGHT, x_pos + PLANE_WIDTH,
                         FLY_HEIGHT + PLANE_HEIGHT, CONFIG_BACKGROUND_COLOR);
    x_pos -= CONFIG_PLANE_DISTANCE_PER_TICK;
    display_fillTriangle(x_pos, y_pos + (PLANE_HEIGHT / HALF),
                         x_pos + PLANE_WIDTH, FLY_HEIGHT, x_pos + PLANE_WIDTH,
                         FLY_HEIGHT + PLANE_HEIGHT, CONFIG_COLOR_PLANE);
    if ((x_pos <= missileDropPoint) && (!hasLaunched)) {
      hasLaunched = true;
      if (missile_is_dead(missile)) {
        missile_init_plane(missile, x_pos + (PLANE_WIDTH / HALF),
                           y_pos + (PLANE_HEIGHT / HALF));
      }
    }
    break;
  case DEAD:
    display_fillTriangle(x_pos, y_pos + (PLANE_HEIGHT / HALF),
                         x_pos + PLANE_WIDTH, FLY_HEIGHT, x_pos + PLANE_WIDTH,
                         FLY_HEIGHT + PLANE_HEIGHT, CONFIG_BACKGROUND_COLOR);
    // Decide Random Wait time
    waitTime = rand() % SPAWN_TICKS_MAX;
    // decide random drop point
    missileDropPoint = rand() % DISPLAY_WIDTH;
    hasLaunched = false;
    // set x and y
    x_pos = DISPLAY_WIDTH;
    y_pos = FLY_HEIGHT;
    break;
  default:
    printf("Plane State machine action error\n");
    break;
  }
}

// Trigger the plane to expode
void plane_explode() {
  planeCurrentState = DEAD;
  display_fillTriangle(x_pos, y_pos + (PLANE_HEIGHT / HALF),
                       x_pos + PLANE_WIDTH, FLY_HEIGHT, x_pos + PLANE_WIDTH,
                       FLY_HEIGHT + PLANE_HEIGHT, CONFIG_BACKGROUND_COLOR);
  // Decide Random Wait time
  waitTime = rand() % SPAWN_TICKS_MAX;
  // decide random drop point
  missileDropPoint = rand() % DISPLAY_WIDTH;
  hasLaunched = false;
  // set x and y
  x_pos = DISPLAY_WIDTH;
  y_pos = FLY_HEIGHT;
}

// Get the XY location of the plane
display_point_t plane_getXY() {
  display_point_t point;
  point.x = x_pos;
  point.y = y_pos + (PLANE_HEIGHT / HALF);
  return point;
}