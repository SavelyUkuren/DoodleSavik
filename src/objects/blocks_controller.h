//
// Created by savely on 10.02.25.
//

#ifndef BLOCKS_CONTROLLER_H
#define BLOCKS_CONTROLLER_H

#include "../common/point.h"
#include "../common/object_size.h"
#include "../common/rect_t.h"
#include "../common/s_random.h"
#include "../config.h"
#include "world.h"
#include "jumpers_controller.h"
#include "player.h"

typedef struct block_t {
    point position;
    object_size size;
} block_t;

extern const int blocks_count;
extern block_t *blocks;

void blocks_init();

void blocks_update(float delta);
void render_blocks(SDL_Renderer *renderer);
void blocks_offset(float offset, float delta);

void player_on_block(player_t *player, float delta);

void destroy_blocks();
#endif //BLOCKS_CONTROLLER_H
