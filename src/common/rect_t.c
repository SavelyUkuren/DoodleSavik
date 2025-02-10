//
// Created by savely on 31.01.25.
//

#include "rect_t.h"


inline bool has_intersection(rect_t a, rect_t b) {
    return !(a.x + a.w <= b.x ||  // a справа от b
             b.x + b.w <= a.x ||  // a слева от b
             a.y + a.h <= b.y ||  // a выше b
             b.y + b.h <= a.y);   // a ниже b
}