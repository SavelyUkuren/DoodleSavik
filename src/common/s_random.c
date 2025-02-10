//
// Created by savely on 01.02.25.
//

#include "s_random.h"

uint64_t xorshift128plus_next(void) {
    static uint64_t state[2];

    if (state[0] == 0 && state[1] == 0) {
        uint64_t seed1 = (uint64_t)time(NULL);
        uint64_t seed2 = seed1 ^ 0x5DEECE66D;
        state[0] = seed1;
        state[1] = seed2;
    }

    uint64_t s1 = state[0];
    const uint64_t s0 = state[1];
    state[0] = s0;

    s1 ^= s1 << 23;
    state[1] = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26);
    return state[1] + s0;
}

uint64_t xorshift128plus_range(uint64_t min, uint64_t max) {
    uint64_t range = max - min + 1;
    uint64_t random = xorshift128plus_next();
    return (random % range) + min;
}

int random_from_range(int min, int max) {
    //return min + (rand() % (max - min + 1));
    return xorshift128plus_range(min, max);
}
