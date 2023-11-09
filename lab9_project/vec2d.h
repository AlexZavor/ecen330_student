#ifndef VEC2D
#define VEC2D

// Vectors
typedef struct {
    int x;
    int y;
} vec2d;

static vec2d makeVec(int x, int y){
    vec2d vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

#endif