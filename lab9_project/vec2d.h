#ifndef VEC2D
#define VEC2D

#include <stdint.h>

// Vectors
typedef struct {
  uint8_t x;
  uint8_t y;
} vec2d;

static vec2d makeVec(uint8_t x, uint8_t y) {
  vec2d vector;
  vector.x = x;
  vector.y = y;
  return vector;
}

#endif