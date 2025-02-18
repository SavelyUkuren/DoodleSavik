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
//#include "jumpers_controller.h"
#include "player.h"
#include "audio_controller.h"

extern player_t player;

#define BLOCKS_COUNT 20

typedef enum block_direction {
    BLOCK_DIR_LEFT,
    BLOCK_DIR_RIGHT
} block_direction;

typedef enum block_type {
    STATIC_BLOCK,
    MOVABLE_BLOCK,
} block_type;

typedef struct jumper_t {
    point position;
    object_size size;
    bool is_currently_uses;
    bool is_jumping;
    float current_time_for_reset;
} jumper_t;

typedef struct block_t {
    // Static block
    point position;
    object_size size;
    block_type type;

    bool has_jumper;
    jumper_t jumper;

    // Movable block
    bool is_movable;
    vec2 velocity;
    block_direction direction;
} block_t;

extern const int blocks_count;
extern block_t blocks[BLOCKS_COUNT];

void blocks_init();

void blocks_update(float delta);
void render_blocks(SDL_Renderer *renderer);
void blocks_offset(float offset, float delta);

void generate_new_blocks(float delta);
void update_movable_blocks(float delta);
void update_jumpers(float delta);

point new_block_position();
float random_dx_velocity();

float map_value(float value, float fromLow, float fromHigh, float toLow, float toHigh);

void player_on_block(player_t *player, float delta);
int ray_intersects_block(rect_t *r_player, rect_t *r_block, float direction);

int movable_block_probability(int by_score);
int jumper_probability(int by_score);

void destroy_blocks();
#endif //BLOCKS_CONTROLLER_H
