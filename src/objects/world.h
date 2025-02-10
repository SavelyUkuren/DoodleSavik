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
#include "blocks_controller.h"
#include "jumpers_controller.h"
#include "background.h"
#include "player.h"

typedef struct world_t {
    player_t *player;
} world_t;

extern SDL_Texture *game_tiles;

void world_init(world_t *world, SDL_Renderer *renderer);

void world_logic(world_t *world, float delta);
void render_world(SDL_Renderer *renderer, world_t *world);

void world_destroy(world_t *world);

// scroll all items and blocks to down if player position y higher some variable
void update_camera(world_t *world, float delta);

bool is_on_top(rect_t *rect1, rect_t *rect2, float velocity_y);


#endif //WORLD_H
