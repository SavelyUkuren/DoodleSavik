//
// Created by savely on 31.01.25.
//

#ifndef RECT_T_H
#define RECT_T_H

#include <stdbool.h>

typedef struct rect_t {
    float x;
    float y;
    float w;
    float h;
} rect_t;

bool has_intersection(rect_t a, rect_t b);

#endif //RECT_T_H
