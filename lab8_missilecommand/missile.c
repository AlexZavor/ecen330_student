#include "missile.h"
#include "config.h"
#include "display.h"
#include <math.h>
#include <stdio.h>

#define ENEMY_SPAWN_Y 25
#define PLAYER_SPAWN_Y 230
#define PLAYER_LAUNCH_POINT_1 80
#define PLAYER_LAUNCH_POINT_DIVIDER_1 120
#define PLAYER_LAUNCH_POINT_2 160
#define PLAYER_LAUNCH_POINT_DIVIDER_2 200
#define PLAYER_LAUNCH_POINT_3 240

#define SPEEDMOD 2

enum missile_st_t {
  FLYING,
  EXPLODE,
  DEEXPLODE,
  DEAD,
};

// global init function for all missile types
static void missile_init_general(missile_t *missile) {
  missile->total_length = sqrt(((missile->y_dest - missile->y_origin) *
                                (missile->y_dest - missile->y_origin)) +
                               ((missile->x_dest - missile->x_origin) *
                                (missile->x_dest - missile->x_origin)));
  missile->x_current = missile->x_origin;
  missile->y_current = missile->y_origin;
  missile->length = 0;
  missile->explode_me = false;
  missile->radius = 0;
  missile->impacted = false;
}

////////// State Machine INIT Functions //////////
// Unlike most state machines that have a single `init` function, our missile
// will have different initializers depending on the missile type.

// Initialize the missile as a dead missile.  This is useful at the start of the
// game to ensure that player and plane missiles aren't moving before they
// should.
void missile_init_dead(missile_t *missile) {
  missile->type = MISSILE_TYPE_ENEMY;
  missile->currentState = DEAD;
  missile->x_origin = 0;
  missile->y_origin = 0;
  missile->x_dest = 0;
  missile->y_dest = 0;
  missile_init_general(missile);
}

// Initialize the missile as an enemy missile.  This will randomly choose the
// origin and destination of the missile.  The origin should be somewhere near
// the top of the screen, and the destination should be the very bottom of the
// screen.
void missile_init_enemy(missile_t *missile) {
  missile->type = MISSILE_TYPE_ENEMY;
  missile->currentState = FLYING;
  missile->x_origin = rand() % DISPLAY_WIDTH;
  missile->y_origin = ENEMY_SPAWN_Y;
  missile->x_dest = rand() % DISPLAY_WIDTH;
  missile->y_dest = DISPLAY_HEIGHT;
  missile_init_general(missile);
}

// Initialize the missile as a player missile.  This function takes an (x, y)
// destination of the missile (where the user touched on the touchscreen).  The
// origin should be the closest "firing location" to the destination (there are
// three firing locations evenly spaced along the bottom of the screen).
void missile_init_player(missile_t *missile, uint16_t x_dest, uint16_t y_dest) {
  missile->type = MISSILE_TYPE_PLAYER;
  missile->currentState = FLYING;
  if (x_dest < PLAYER_LAUNCH_POINT_DIVIDER_1) {
    missile->x_origin = PLAYER_LAUNCH_POINT_1;
  } else if (x_dest < PLAYER_LAUNCH_POINT_DIVIDER_2) {
    missile->x_origin = PLAYER_LAUNCH_POINT_2;
  } else {
    missile->x_origin = PLAYER_LAUNCH_POINT_3;
  }
  missile->y_origin = PLAYER_SPAWN_Y;
  missile->x_dest = x_dest;
  missile->y_dest = y_dest;
  missile_init_general(missile);
}

// Initialize the missile as a plane missile.  This function takes an (x, y)
// location of the plane which will be used as the origin.  The destination can
// be randomly chosed along the bottom of the screen.
void missile_init_plane(missile_t *missile, int16_t plane_x, int16_t plane_y) {
  missile->type = MISSILE_TYPE_PLANE;
  missile->currentState = FLYING;
  missile->x_origin = plane_x;
  missile->y_origin = plane_y;
  missile->x_dest = rand() % DISPLAY_WIDTH;
  missile->y_dest = DISPLAY_HEIGHT;
  missile_init_general(missile);
}

////////// State Machine TICK Function //////////
void missile_tick(missile_t *missile) {
  // Transition
  switch (missile->currentState) {
  case FLYING:
    // Player missiles explode when reach destination, while enemys die and
    // impact.
    if (missile->length >= missile->total_length) {
      if (missile->type == MISSILE_TYPE_PLAYER) {
        missile->currentState = EXPLODE;
        display_drawLine(missile->x_origin, missile->y_origin,
                         missile->x_current, missile->y_current,
                         CONFIG_BACKGROUND_COLOR);
      } else {
        missile->currentState = DEAD;
        display_drawLine(missile->x_origin, missile->y_origin,
                         missile->x_current, missile->y_current,
                         CONFIG_BACKGROUND_COLOR);
        missile->impacted = true;
      }
    }
    break;
  case EXPLODE:
    if (missile->radius >= CONFIG_EXPLOSION_MAX_RADIUS) {
      missile->currentState = DEEXPLODE;
    }
    break;
  case DEEXPLODE:
    if (missile->radius <= 0) {
      missile->currentState = DEAD;
    }
    break;
  case DEAD:
    // Do nothing while missile is dead
    break;
  default:
    // Error State
    printf("Missile State machine transition error - missile type = %d\n",
           missile->type);
    break;
  }
  // Action
  switch (missile->currentState) {
  case FLYING:
    // erase previous line
    display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                     missile->y_current, CONFIG_BACKGROUND_COLOR);

    // add appropriate distance
    switch (missile->type) {
    case MISSILE_TYPE_PLAYER:
      missile->length += CONFIG_PLAYER_MISSILE_DISTANCE_PER_TICK * SPEEDMOD;
      break;
    case MISSILE_TYPE_ENEMY:
      missile->length += CONFIG_ENEMY_MISSILE_DISTANCE_PER_TICK * SPEEDMOD;
      break;
    case MISSILE_TYPE_PLANE:
      missile->length += CONFIG_ENEMY_MISSILE_DISTANCE_PER_TICK * SPEEDMOD;
      break;
    }

    // Calculate new current x and y
    double percent = missile->length / missile->total_length;
    missile->x_current =
        missile->x_origin + (percent * (missile->x_dest - missile->x_origin));
    missile->y_current =
        missile->y_origin + (percent * (missile->y_dest - missile->y_origin));

    // print new line in appropriate color
    switch (missile->type) {
    case MISSILE_TYPE_PLAYER:
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, CONFIG_COLOR_PLAYER);
      break;
    case MISSILE_TYPE_ENEMY:
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, CONFIG_COLOR_ENEMY);
      break;
    case MISSILE_TYPE_PLANE:
      display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                       missile->y_current, CONFIG_COLOR_PLANE);
      break;
    }

    break;
  case EXPLODE:
    // Increment Radius
    missile->radius += CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK * SPEEDMOD;
    // print circle in appropriate color
    switch (missile->type) {
    case MISSILE_TYPE_PLAYER:
      display_fillCircle(missile->x_current, missile->y_current,
                         missile->radius, CONFIG_COLOR_PLAYER);
      break;
    case MISSILE_TYPE_ENEMY:
      display_fillCircle(missile->x_current, missile->y_current,
                         missile->radius, CONFIG_COLOR_ENEMY);
      break;
    case MISSILE_TYPE_PLANE:
      display_fillCircle(missile->x_current, missile->y_current,
                         missile->radius, CONFIG_COLOR_PLANE);
      break;
    }
    break;
  case DEEXPLODE:
    // Erase old Circle
    display_fillCircle(missile->x_current, missile->y_current, missile->radius,
                       CONFIG_BACKGROUND_COLOR);
    // Decrement
    missile->radius -= CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK * SPEEDMOD;
    // print circle in appropriate color
    switch (missile->type) {
    case MISSILE_TYPE_PLAYER:
      display_fillCircle(missile->x_current, missile->y_current,
                         missile->radius, CONFIG_COLOR_PLAYER);
      break;
    case MISSILE_TYPE_ENEMY:
      display_fillCircle(missile->x_current, missile->y_current,
                         missile->radius, CONFIG_COLOR_ENEMY);
      break;
    case MISSILE_TYPE_PLANE:
      display_fillCircle(missile->x_current, missile->y_current,
                         missile->radius, CONFIG_COLOR_PLANE);
      break;
    }
    break;
  case DEAD:
    // Do nothing while missile is dead
    break;
  default:
    // Error State
    printf("Missile State machine action error - missile type = %d\n",
           missile->type);
    break;
  }
}

// Return whether the given missile is dead.
bool missile_is_dead(missile_t *missile) {
  return (missile->currentState == DEAD);
}

// Return whether the given missile is exploding.  This is needed when detecting
// whether a missile hits another exploding missile.
bool missile_is_exploding(missile_t *missile) {
  return ((missile->currentState == EXPLODE) ||
          (missile->currentState == DEEXPLODE));
}

// Return whether the given missile is flying.
bool missile_is_flying(missile_t *missile) {
  return (missile->currentState == FLYING);
}

// Used to indicate that a flying missile should be detonated.  This occurs when
// an enemy or plane missile is located within an explosion zone.
void missile_trigger_explosion(missile_t *missile) {
  if (missile->currentState == FLYING) {
    display_drawLine(missile->x_origin, missile->y_origin, missile->x_current,
                     missile->y_current, CONFIG_BACKGROUND_COLOR);
    missile->currentState = EXPLODE;
  }
}
