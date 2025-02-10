//
// Created by savely on 10.02.25.
//

#ifndef JUMPERS_CONTROLLER_H
#define JUMPERS_CONTROLLER_H

#include "../common/point.h"
#include "../common/object_size.h"
#include "../common/rect_t.h"
#include "../common/s_random.h"
#include "../config.h"
#include "world.h"
#include "player.h"

typedef struct jumper_t {
    point position;
    object_size size;
    bool is_currently_uses;
    bool is_jumping;
    float current_time_for_reset;
} jumper_t;

extern const int jumpers_count;
extern jumper_t *jumpers;

void jumpers_init();
void jumpers_update(float delta);
void render_jumpers(SDL_Renderer *renderer);
void jumpers_offset(float offset, float delta);

void player_on_jumper(player_t *player, float delta);

void destroy_jumpers();

#endif //JUMPERS_CONTROLLER_H
