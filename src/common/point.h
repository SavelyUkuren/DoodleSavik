#ifndef POINT_H
#define POINT_H

#include <math.h>

typedef struct point {
    float x;
    float y;
} point;

float distance(point a, point b);

#endif // POINT_H
