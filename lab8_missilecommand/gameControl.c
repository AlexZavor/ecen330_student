#include "gameControl.h"
#include "config.h"
#include "display.h"
#include "missile.h"
#include "plane.h"
#include "touchscreen.h"

#include <stdint.h>
#include <stdio.h>

#define TEXT_SIZE 2
#define SCORE_Y 3
#define SHOT_X 5
#define SHOT_NUM_X 65
#define IMPACT_X 160
#define IMPACT_NUM_X 270
#define HALF 2

static missile_t missiles[CONFIG_MAX_TOTAL_MISSILES];
static missile_t *enemy_missiles = &(missiles[0]);
static missile_t *player_missiles = &(missiles[CONFIG_MAX_ENEMY_MISSILES + 1]);

static bool missileSection;

static uint16_t missilesLaunched_prev;
static uint16_t missilesLaunched;
static uint16_t impacts_prev;
static uint16_t impacts;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void gameControl_init() {
  display_fillScreen(CONFIG_BACKGROUND_COLOR);
  display_setTextColor(DISPLAY_WHITE);
  display_setTextSize(TEXT_SIZE);
  display_setCursor(SHOT_X, SCORE_Y);
  display_println("Shot:");
  display_setCursor(IMPACT_X, SCORE_Y);
  display_println("Impacted:");
#ifdef LAB8_M3
  plane_init(&missiles[CONFIG_MAX_ENEMY_MISSILES]);
#endif

  // Init all missiles as dead
  for (uint16_t i = 0; i < CONFIG_MAX_TOTAL_MISSILES; i++)
    missile_init_dead(&missiles[i]);

  missileSection = 0;
  missilesLaunched_prev = 1;
  missilesLaunched = 0;
  impacts_prev = 1;
  impacts = 0;
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() {

  // Tick half of the missiles
  if (missileSection) {
    for (uint16_t i = 0; i < CONFIG_MAX_TOTAL_MISSILES / HALF; i++) {
      missile_tick(&missiles[i]);
    }
  } else {
    for (uint16_t i = CONFIG_MAX_TOTAL_MISSILES / HALF;
         i < CONFIG_MAX_TOTAL_MISSILES; i++) {
      missile_tick(&missiles[i]);
    }
  }
  // Tick Plane
#ifdef LAB8_M3
  plane_tick();
#endif
  missileSection = !missileSection;

  // Check for dead enemy missiles and re-initialize
  for (uint16_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++) {
    if (missile_is_dead(&enemy_missiles[i])) {
      if (enemy_missiles[i].impacted) {
        impacts++;
      }
      missile_init_enemy(&enemy_missiles[i]);
    }
  }
  // Check for impact of plane missile
  if (enemy_missiles[CONFIG_MAX_ENEMY_MISSILES].impacted) {
    impacts++;
    enemy_missiles[CONFIG_MAX_ENEMY_MISSILES].impacted = false;
  }

  // Launch player missile for touch
  if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
    touchscreen_ack_touch();
    for (uint16_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++) {
      if (missile_is_dead(&player_missiles[i])) {
        display_point_t destination = touchscreen_get_location();
        missile_init_player(&player_missiles[i], destination.x, destination.y);
        missilesLaunched++;
        break;
      }
    }
  }

  // Check for collision
  for (uint16_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES + 1; i++) {
    for (uint16_t j = 0; j < CONFIG_MAX_TOTAL_MISSILES; j++) {
      // check if pair is flying
      if (!missile_is_flying(&enemy_missiles[i]))
        continue;
      // check if pair is exploding
      if (!missile_is_exploding(&missiles[j]))
        continue;

      uint16_t dy = abs((int16_t)missiles[j].y_current -
                        (int16_t)enemy_missiles[i].y_current);
      uint16_t dx = abs((int16_t)missiles[j].x_current -
                        (int16_t)enemy_missiles[i].x_current);
      if ((dy * dy) + (dx * dx) <= missiles[j].radius * missiles[j].radius)
        missile_trigger_explosion(&enemy_missiles[i]);
    }
  }

#ifdef LAB8_M3
  // Plane Collision
  for (uint16_t j = 0; j < CONFIG_MAX_TOTAL_MISSILES; j++) {
    if (!missile_is_exploding(&missiles[j]))
      continue;

    display_point_t plane_location = plane_getXY();
    uint16_t dy =
        abs((int16_t)missiles[j].y_current - (int16_t)plane_location.y);
    uint16_t dx =
        abs((int16_t)missiles[j].x_current - (int16_t)plane_location.x);
    if ((dy * dy) + (dx * dx) <= missiles[j].radius * missiles[j].radius)
      plane_explode();
  }
#endif

  // Draw Stats
  if (impacts_prev != impacts) {
    display_setTextColor(CONFIG_BACKGROUND_COLOR);
    display_setCursor(IMPACT_NUM_X, SCORE_Y);
    display_printDecimalInt(impacts_prev);
    display_setTextColor(DISPLAY_RED);
    display_setCursor(IMPACT_NUM_X, SCORE_Y);
    display_printDecimalInt(impacts);
    impacts_prev = impacts;
  }
  if (missilesLaunched_prev != missilesLaunched) {
    display_setTextColor(CONFIG_BACKGROUND_COLOR);
    display_setCursor(SHOT_NUM_X, SCORE_Y);
    display_printDecimalInt(missilesLaunched_prev);
    display_setTextColor(DISPLAY_GREEN);
    display_setCursor(SHOT_NUM_X, SCORE_Y);
    display_printDecimalInt(missilesLaunched);
    missilesLaunched_prev = missilesLaunched;
  }
  display_setTextColor(DISPLAY_WHITE);
  display_setCursor(SHOT_X, SCORE_Y);
  display_println("Shot:");
  display_setCursor(IMPACT_X, SCORE_Y);
  display_println("Impacted:");
}