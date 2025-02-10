//
// Created by savely on 31.01.25.
//

#ifndef WORLD_H
#define WORLD_H

#include <stdio.h>

#include "../libs/SDL_image.h"
#include "../common/point.h"
#include "../common/object_size.h"
#include "../common/rect_t.h"
#include "../config.h"
#include "../common/vec2.h"
#include "../common/s_random.h"
#include "../assets_p.h"

#include "player.h"

typedef struct block_t {
    point position;
    object_size size;
} block_t;

typedef struct jumper_t {
    point position;
    object_size size;
    bool is_currently_uses;
    bool is_jumping;
    float current_time_for_reset;
} jumper_t;

typedef struct world_t {
    player_t *player;
} world_t;

void world_init(world_t *world, SDL_Renderer *renderer);

void world_logic(world_t *world, float delta);
void render_world(SDL_Renderer *renderer, world_t *world);

void world_destroy(world_t *world);

// check if player landed on block
void player_on_block_update(world_t *world, float delta);

// check if player landed on jumper
void player_on_jumper_update(world_t *world, float delta);

// scroll all items and blocks to down if player position y higher some variable
void update_camera(world_t *world, float delta);

// if blocks or items out of screen
void update_blocks_positions(world_t *world, float delta);
void update_jumper_positions(world_t *world, float delta);

bool is_on_top(rect_t *rect1, rect_t *rect2, world_t *world);

void update_background(world_t *world, float delta);
void render_background(SDL_Renderer *renderer, world_t *world);

#endif //WORLD_H
