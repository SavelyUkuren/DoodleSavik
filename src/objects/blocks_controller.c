//
// Created by savely on 10.02.25.
//

#include "blocks_controller.h"

#include <stdlib.h>

block_t blocks[BLOCKS_COUNT];

float movable_block_generator_time = 0;

void blocks_init() {

    point last_block_pos;
    last_block_pos.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
    last_block_pos.y = WINDOW_HEIGHT - 20;
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        block_t block;
        int r = random_from_range(50, 100);
        block.position.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
        block.position.y = last_block_pos.y - (float)random_from_range(80, 100);
        last_block_pos = block.position;
        block.size.w = 59 * GAME_SCALE;
        block.size.h = 17 * GAME_SCALE;
        blocks[i] = block;
        blocks[i].type = STATIC_BLOCK;
        blocks[i].is_movable = false;
        blocks[i].velocity.dx = 0;
        blocks[i].velocity.dy = 0;

        jumper_t jumper;
        jumper.size.w = 18 * GAME_SCALE;
        jumper.size.h = 12 * GAME_SCALE;
        jumper.position.x = -1000;
        jumper.position.y = -1000;
        jumper.is_currently_uses = false;
        jumper.current_time_for_reset = 0;
        jumper.is_jumping = false;
        blocks[i].has_jumper = false;
        blocks[i].jumper = jumper;
    }

}

void blocks_update(float delta) {

    generate_new_blocks(delta);
    update_movable_blocks(delta);
    update_jumpers(delta);
}

void blocks_offset(float offset, float delta) {
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        blocks[i].position.y += offset;
        if (blocks[i].has_jumper) {
            blocks[i].jumper.position.y += offset;
        }
    }
}

void player_on_block(player_t *player, float delta) {
    player->is_on_block = false;
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        if (player->velocity.dy > 0) {
            block_t *block = &blocks[i];

            rect_t r_player = {
                player->position.x,
                player->position.y,
                player->size.w,
                player->size.h
            };

            if (block->has_jumper) {
                rect_t r_jumper = {
                    block->jumper.position.x,
                    block->jumper.position.y,
                    block->size.w,
                    block->size.h
                };

                if (is_on_top(&r_player, &r_jumper, player->velocity.dy)) {
                    player_jump(player, PLAYER_ON_JUMPER_FORCE);
                    block->jumper.is_jumping = true;
                    player->position.y = r_jumper.y - r_player.h;
                    play_jumper_sound();
                }

            } else {
                rect_t r_block = {
                    block->position.x,
                    block->position.y,
                    block->size.w,
                    block->size.h
                };

                if (is_on_top(&r_player, &r_block, player->velocity.dy)) {
                    player_jump(player, PLAYER_JUMP_FORCE);
                    player->position.y = r_block.y - r_player.h;
                    play_jump_sound();
                }
            }

        }
    }
}

void render_blocks(SDL_Renderer *renderer) {
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        SDL_FRect r1 = {blocks[i].position.x,
                        blocks[i].position.y,
                        blocks[i].size.w,
                        blocks[i].size.h};

        if (blocks[i].type == STATIC_BLOCK) {

            SDL_FRect src_rect = {0, 0, 118, 34};
            SDL_RenderTexture(renderer, game_tiles, &src_rect, &r1);

        } else if (blocks[i].type == MOVABLE_BLOCK) {

            SDL_FRect src_rect =  {0, 34, 118, 34};
            SDL_RenderTexture(renderer, game_tiles, &src_rect, &r1);

        }

        if (blocks[i].has_jumper) {
            SDL_FRect jumper_r = {
                blocks[i].jumper.position.x,
                blocks[i].jumper.position.y,
                blocks[i].jumper.size.w,
                blocks[i].jumper.size.h
            };

            SDL_FRect src_rect;
            if (blocks[i].jumper.is_jumping) {
                src_rect.x = 806;
                src_rect.y = 229;
                src_rect.w = 36;
                src_rect.h = 57;

                jumper_r.h = 30;
                jumper_r.y -= (float)(16 / GAME_SCALE);
            } else {
                src_rect.x = 806;
                src_rect.y = 196;
                src_rect.w = 36;
                src_rect.h = 25;
            }
            SDL_RenderTexture(renderer, game_tiles, &src_rect, &jumper_r);
        }
    }

}

void generate_new_blocks(float delta) {
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        block_t block = blocks[i];
        if (block.position.y > WINDOW_HEIGHT) {
            blocks[i].type = STATIC_BLOCK;
            blocks[i].is_movable = false;
            blocks[i].velocity.dx = 0;
            blocks[i].position = new_block_position();
            blocks[BLOCKS_COUNT - 1] = blocks[i];
            blocks[i].has_jumper = false;

            if (random_from_range(0, 100) < movable_block_probability(player.score)) {
                blocks[i].type = MOVABLE_BLOCK;
                blocks[i].is_movable = true;
                blocks[i].direction = BLOCK_DIR_LEFT;
                blocks[i].velocity.dx = random_dx_velocity();
            }

            if (random_from_range(0, 100) < JUMPER_PROBALITY && !blocks[i].has_jumper) {
                blocks[i].has_jumper = true;
                float random_x = random_from_range(blocks[i].position.x, blocks[i].position.x + blocks[i].size.w - blocks[i].jumper.size.w);
                blocks[i].jumper.position.x = random_x;
                blocks[i].jumper.position.y = blocks[i].position.y - blocks[i].jumper.size.h + 4;
            }


        }
    }
}

void update_movable_blocks(float delta) {
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        if (blocks[i].type == MOVABLE_BLOCK) {
            if (blocks[i].position.x + blocks[i].size.w + 12 >= WINDOW_WIDTH) {
                blocks[i].direction = BLOCK_DIR_LEFT;
            }
            if (blocks[i].position.x - 12 <= 0) {
                blocks[i].direction = BLOCK_DIR_RIGHT;
            }

            if (blocks[i].direction == BLOCK_DIR_LEFT) {
                blocks[i].velocity.dx *= blocks[i].velocity.dx < 0 ? 1 : -1;
            }
            if (blocks[i].direction == BLOCK_DIR_RIGHT) {
                blocks[i].velocity.dx *= blocks[i].velocity.dx > 0 ? 1 : -1;
            }
            blocks[i].position.x += blocks[i].velocity.dx * delta;
            if (blocks[i].has_jumper) {
                blocks[i].jumper.position.x += blocks[i].velocity.dx * delta;
            }
        }
    }
}

void update_jumpers(float delta) {
    for (int i = 0 ; i < BLOCKS_COUNT ; i++) {
        if (blocks[i].has_jumper) {
            if (blocks[i].position.y > WINDOW_HEIGHT) {
                blocks[i].has_jumper = false;
                blocks[i].position.x = -1000;
                blocks[i].position.y = -1000;
                blocks[i].jumper.is_jumping = false;
                blocks[i].jumper.current_time_for_reset = 0;
            }

            if (blocks[i].jumper.is_jumping && blocks[i].jumper.current_time_for_reset < 0.5) {
                blocks[i].jumper.current_time_for_reset += delta;
            } else {
                blocks[i].jumper.current_time_for_reset = 0;
                blocks[i].jumper.is_jumping = false;
            }
        }
    }
}

point new_block_position() {
    float min_distance = fmin(map_value(player.score, 0, 10000, 30, 120), 120);
    point p;
    p.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
    p.y = blocks[BLOCKS_COUNT - 1].position.y - (float)random_from_range(min_distance, 130);
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

int movable_block_probability(int by_score) {
    return fmin(map_value(player.score, 0, 10000, 20, 60), 60);
}

int jumper_probability(int by_score) {

}

void destroy_blocks() {

}
