//
// Created by savely on 31.01.25.
//

#include "point.h"

float distance(point a, point b) {
    return sqrt((pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}