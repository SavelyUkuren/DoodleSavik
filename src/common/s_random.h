//
// Created by savely on 01.02.25.
//

#ifndef S_RANDOM_H
#define S_RANDOM_H
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

uint64_t xorshift128plus_next(void);
uint64_t xorshift128plus_range(uint64_t min, uint64_t max);

int random_from_range(int min, int max);

#endif //S_RANDOM_H
