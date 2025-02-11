//
// Created by savely on 10.02.25.
//

#include "blocks_controller.h"

#include <stdlib.h>


const int blocks_count = 20;
block_t *blocks;

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
}

void blocks_update(float delta) {
    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block = blocks[i];
        if (block.position.y > WINDOW_HEIGHT) {
            blocks[i].position.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
            blocks[i].position.y = blocks[blocks_count - 1].position.y - (float)random_from_range(80, 100);
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
}

void blocks_offset(float offset, float delta) {
    for (int i = 0 ; i < blocks_count ; i++) {
        blocks[i].position.y += offset;
    }
}

void player_on_block(player_t *player, float delta) {
    player->is_on_block = false;
    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block = blocks[i];

        if (player->velocity.dy > 0) {

            rect_t r_player = {
                player->position.x,
                player->position.y,
                player->size.w,
                player->size.h
            };

            rect_t r_block = {
                block.position.x,
                block.position.y,
                block.size.w,
                block.size.h
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
}

void destroy_blocks() {
    free(blocks);
}