//
// Created by savely on 10.02.25.
//

#include "blocks_controller.h"

#include <stdlib.h>


const int blocks_count = 20;
block_t *blocks;

const int movable_blocks_count = 5;
movable_block_t *movable_blocks;

float movable_block_generator_time = 0;

void blocks_init() {

    blocks = malloc(sizeof(block_t) * blocks_count);

    point last_block_pos;
    last_block_pos.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
    last_block_pos.y = WINDOW_HEIGHT - 20;
    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block;
        int r = random_from_range(50, 100);
        block.position.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
        block.position.y = last_block_pos.y - (float)random_from_range(80, 100);
        last_block_pos = block.position;
        block.size.w = 59 * GAME_SCALE;
        block.size.h = 17 * GAME_SCALE;
        blocks[i] = block;
    }

    movable_blocks = malloc(sizeof(movable_block_t) * movable_blocks_count);
    for (int i = 0 ; i < movable_blocks_count ; i++) {
        movable_block_t block;
        block.position.x = -100;
        block.position.y = -100;
        block.is_currently_uses = false;
        block.direction = BLOCK_DIR_LEFT;
        block.size.w = 59 * GAME_SCALE;
        block.size.h = 17 * GAME_SCALE;
        block.velocity.dx = 0;
        block.velocity.dy = 0;
        movable_blocks[i] = block;
    }

}

void blocks_update(float delta) {

    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block = blocks[i];
        if (block.position.y > WINDOW_HEIGHT) {
            blocks[i].position = new_block_position();
            blocks[blocks_count - 1] = blocks[i];

            //with some probability(JUMPER_PROBALITY) we add jumper to block
            if (random_from_range(0, 100) < JUMPER_PROBALITY) {
                for (int j = 0 ; j < jumpers_count ; j++) {
                    if (!jumpers[j].is_currently_uses) {
                        float random_jumper_pos_x = (float)random_from_range(0, block.size.w - jumpers[j].size.w);
                        jumpers[j].position.x = blocks[i].position.x + random_jumper_pos_x;
                        jumpers[j].position.y = blocks[i].position.y - jumpers[j].size.h + 3;
                        jumpers[j].is_currently_uses = true;
                        break;
                    }
                }
            }

        }
    }

    movable_block_generator_time += delta;
    if (movable_block_generator_time >= 1) {
        float probability = fmin(map_value(player.score, 0, 6000, 30, 80), 80);
        if (random_from_range(0, 100) < probability) {
            int block_index = -1;
            for (int j = 0 ; j < blocks_count; j++) {
                if (blocks[j].position.y < 0) {
                    block_index = j;
                    break;
                }
            }

            if (block_index != -1) {
                int movable_block_index = -1;
                for (int j = 0 ; j < movable_blocks_count; j++) {
                    if (!movable_blocks[j].is_currently_uses) {
                        movable_block_index = j;
                        break;
                    }
                }
                if (movable_block_index != -1) {
                    movable_blocks[movable_block_index].position = blocks[block_index].position;
                    movable_blocks[movable_block_index].is_currently_uses = true;
                    movable_blocks[movable_block_index].velocity.dx = random_dx_velocity();
                    blocks[block_index].position = new_block_position();
                    blocks[blocks_count - 1] = blocks[block_index];
                }
            }
        }
        movable_block_generator_time = 0;
    }

    for (int i = 0 ; i < movable_blocks_count ; i++) {
        if (movable_blocks[i].is_currently_uses) {
            point p = movable_blocks[i].position;
            object_size size = movable_blocks[i].size;

            if (p.x + size.w + 12 >= WINDOW_WIDTH) {
                movable_blocks[i].direction = BLOCK_DIR_LEFT;
            }
            if (p.x - 12 <= 0) {
                movable_blocks[i].direction = BLOCK_DIR_RIGHT;
            }

            if (movable_blocks[i].direction == BLOCK_DIR_LEFT) {
                movable_blocks[i].velocity.dx *= movable_blocks[i].velocity.dx < 0 ? 1 : -1;
            }
            if (movable_blocks[i].direction == BLOCK_DIR_RIGHT) {
                movable_blocks[i].velocity.dx *= movable_blocks[i].velocity.dx > 0 ? 1 : -1;
            }
            movable_blocks[i].position.x += movable_blocks[i].velocity.dx * delta;

            if (movable_blocks[i].position.y > WINDOW_HEIGHT) {
                movable_blocks[i].is_currently_uses = false;
                movable_blocks[i].position.x = -1000;
                movable_blocks[i].position.y = -1000;
                movable_blocks[i].velocity.dx = 0;
            }
        }

    }


}

void blocks_offset(float offset, float delta) {
    for (int i = 0 ; i < blocks_count ; i++) {
        blocks[i].position.y += offset;
    }
    for (int i = 0 ; i < movable_blocks_count ; i++) {
        movable_blocks[i].position.y += offset;
    }
}

void player_on_block(player_t *player, float delta) {
    player->is_on_block = false;
    for (int i = 0 ; i < blocks_count ; i++) {

        if (player->velocity.dy > 0) {
            block_t *block = &blocks[i];

            rect_t r_player = {
                player->position.x,
                player->position.y,
                player->size.w,
                player->size.h
            };

            rect_t r_block = {
                block->position.x,
                block->position.y,
                block->size.w,
                block->size.h
            };

            if (is_on_top(&r_player, &r_block, player->velocity.dy)) {
                player->velocity.dy = PLAYER_JUMP_FORCE;
                player->position.y = r_block.y - r_player.h;
                player->is_on_block = true;
                play_jump_sound();
            }

        }
    }
    if (player->velocity.dy > 0) {
        for (int i = 0 ; i < movable_blocks_count ; i++) {
            rect_t r_player = {
                player->position.x,
                player->position.y,
                player->size.w,
                player->size.h
            };

            rect_t r_block = {
                movable_blocks[i].position.x,
                movable_blocks[i].position.y,
                movable_blocks[i].size.w,
                movable_blocks[i].size.h
            };

            if (is_on_top(&r_player, &r_block, player->velocity.dy)) {
                player->velocity.dy = PLAYER_JUMP_FORCE;
                player->position.y = r_block.y - r_player.h;
                player->is_on_block = true;
                play_jump_sound();
            }
        }
    }
}

void render_blocks(SDL_Renderer *renderer) {
    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block = blocks[i];
        SDL_FRect r1 = {block.position.x,
                        block.position.y,
                        block.size.w,
                        block.size.h};

        SDL_FRect src_rect = {0, 0, 118, 34};
        SDL_RenderTexture(renderer, game_tiles, &src_rect, &r1);
        //SDL_RenderFillRect(renderer, &r1);
    }

    for (int i = 0 ; i < movable_blocks_count ; i++) {

        SDL_FRect mov_block_rect = {
            movable_blocks[i].position.x,
            movable_blocks[i].position.y,
            movable_blocks[i].size.w,
            movable_blocks[i].size.h};

        SDL_FRect src_rect = {0, 34, 118, 34};
        SDL_RenderTexture(renderer, game_tiles, &src_rect, &mov_block_rect);

    }

}

point new_block_position() {
    float min_distance = fmin(map_value(player.score, 0, 10000, 30, 120), 120);
    point p;
    p.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
    p.y = blocks[blocks_count - 1].position.y - (float)random_from_range(min_distance, 130);
    return p;
}

float random_dx_velocity() {
    int sign = (rand() % 2) * 2 - 1;
    return sign * random_from_range(MOVABLE_BLOCK_VELOCITY_MIN, MOVABLE_BLOCK_VELOCITY_MAX);
}

float map_value(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    return toLow + (toHigh - toLow) * ((value - fromLow) / (fromHigh - fromLow));
}

int ray_intersects_block(rect_t *r_player, rect_t *r_block, float direction) {
    if (direction > 0) {
        return (r_player->y + r_player->h <= r_block->y && r_player->y + r_player->h + r_player->h > r_block->y);
    } else {
        return (r_player->y >= r_block->y + r_block->h && r_player->y - r_player->h < r_block->y + r_block->h);
    }
}

void destroy_blocks() {
    free(blocks);
    free(movable_blocks);
}