//
// Created by savely on 31.01.25.
//

#include "world.h"

#include <stdlib.h>

block_t temp_block1;
block_t temp_block2;

int r = 0;
int g = 255;

const int blocks_count = 20;
block_t *blocks;

jumper_t temp_jumper;

const int jumpers_count = 3;
jumper_t *jumpers;

float world_offset_y = 0;

point background1_pos;
point background2_pos;
float background_h;
SDL_Texture *t_background;

SDL_Texture *game_tiles;

void world_init(world_t *world, SDL_Renderer *renderer) {
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

    jumpers = malloc(sizeof(jumper_t) * jumpers_count);
    for (int i = 0 ; i < jumpers_count ; i++) {
        jumper_t jumper;
        jumper.position.x = -100;
        jumper.position.y = -100;
        jumper.size.w = 18 * GAME_SCALE;
        jumper.size.h = 12 * GAME_SCALE;
        jumper.is_currently_uses = false;
        jumper.is_jumping = false;
        jumper.current_time_for_reset = 0;
        jumpers[i] = jumper;
    }

    temp_jumper.position.x = WINDOW_WIDTH-32;
    temp_jumper.position.y = WINDOW_HEIGHT-32;
    temp_jumper.size.w = 32;
    temp_jumper.size.h = 32;

    t_background = IMG_LoadTexture(renderer, BACKGROUND_TEXTURE);
    if (!t_background) {
        printf("Failed to load '%s' texture\n", BACKGROUND_TEXTURE);
    }
    float scaleX = (float)WINDOW_WIDTH / (float)t_background->w;
    float scaleY = (float)WINDOW_HEIGHT / (float)t_background->h;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    background_h = scale * t_background->h * 1.3;
    background2_pos.y = -background_h;

    game_tiles = IMG_LoadTexture(renderer, GAME_TILES);
    if (!game_tiles) {
        printf("Failed to load '%s' texture\n", GAME_TILES);
    }
}

void world_logic(world_t *world, float delta) {

    update_background(world, delta);

    player_on_block_update(world, delta);
    update_camera(world, delta);
    update_blocks_positions(world, delta);

    player_on_jumper_update(world, delta);
    update_jumper_positions(world, delta);

}

void render_world(SDL_Renderer *renderer, world_t *world) {

    render_background(renderer, world);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0 ; i < jumpers_count ; i++) {
        SDL_FRect r_jumper = {
            jumpers[i].position.x,
            jumpers[i].position.y,
            jumpers[i].size.w,
            jumpers[i].size.h
        };

        SDL_FRect src_rect;
        if (jumpers[i].is_jumping) {
            src_rect.x = 806;
            src_rect.y = 229;
            src_rect.w = 36;
            src_rect.h = 57;

            r_jumper.h = 28;
            r_jumper.y -= (float)(15 / GAME_SCALE);
        } else {
            src_rect.x = 806;
            src_rect.y = 196;
            src_rect.w = 36;
            src_rect.h = 25;
        }
        SDL_RenderTexture(renderer, game_tiles, &src_rect, &r_jumper);
    }

}

void world_destroy(world_t *world) {
    SDL_DestroyTexture(t_background);
    SDL_DestroyTexture(game_tiles);
    free(blocks);
    free(jumpers);
}

bool is_on_top(rect_t *rect1, rect_t *rect2, world_t *world) {
    return (rect1->y + rect1->h <= rect2->y &&
            rect1->y + rect1->h + world->player->velocity.dy > rect2->y &&
            rect1->x + rect1->w > rect2->x &&
            rect1->x < rect2->x + rect2->w);
}

void player_on_block_update(world_t *world, float delta) {
    world->player->is_on_block = false;
    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block = blocks[i];

        if (world->player->velocity.dy > 0) {

            rect_t r_player = {
                world->player->position.x,
                world->player->position.y,
                world->player->size.w,
                world->player->size.h
            };

            rect_t r_block = {
                block.position.x,
                block.position.y,
                block.size.w,
                block.size.h
            };

            if (is_on_top(&r_player, &r_block, world)) {
                world->player->velocity.dy = PLAYER_JUMP_FORCE;
                world->player->position.y = r_block.y - r_player.h;
                world->player->is_on_block = true;
            }

        }

    }
}

void update_camera(world_t *world, float delta) {

    world_offset_y = -world->player->velocity.dy * 100 * delta;

    if (world->player->position.y < WINDOW_HEIGHT / 3) {
        world->player->position.y = WINDOW_HEIGHT / 3;

        for (int i = 0 ; i < blocks_count ; i++) {
            blocks[i].position.y += world_offset_y;
        }

        for (int i = 0; i < jumpers_count ; i++) {
            jumpers[i].position.y += world_offset_y;
        }

        world->player->score += 1;

        background1_pos.y += world_offset_y;
        background2_pos.y += world_offset_y;
    }

    if (world->player->position.y + world->player->size.h > WINDOW_HEIGHT &&
        world->player->position.x < WINDOW_HEIGHT * 2) {
        for (int i = 0 ; i < blocks_count ; i++) {
            blocks[i].position.y += world_offset_y;
        }

        for (int i = 0; i < jumpers_count ; i++) {
            jumpers[i].position.y += world_offset_y;
        }

        background1_pos.y -= world_offset_y;
        background2_pos.y -= world_offset_y;
    }

}

void update_blocks_positions(world_t *world, float delta) {
    for (int i = 0 ; i < blocks_count ; i++) {
        block_t block = blocks[i];
        if (block.position.y > WINDOW_HEIGHT) {
            blocks[i].position.x = (float)random_from_range(8, WINDOW_WIDTH - 100 - 8);
            blocks[i].position.y = blocks[blocks_count - 1].position.y - (float)random_from_range(80, 100);
            blocks[blocks_count - 1] = blocks[i];

            // with some probability(JUMPER_PROBALITY) we add jumper to block
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

void player_on_jumper_update(world_t *world, float delta) {
    if (world->player->velocity.dy > 0) {
        rect_t r_player = {
            world->player->position.x,
            world->player->position.y,
            world->player->size.w,
            world->player->size.h
        };

        for (int i = 0 ; i < jumpers_count; i++) {

            rect_t r_jumper = {
                jumpers[i].position.x,
                jumpers[i].position.y,
                jumpers[i].size.w,
                jumpers[i].size.h
            };

            if (is_on_top(&r_player, &r_jumper, world)) {
                world->player->velocity.dy = PLAYER_ON_JUMPER_FORCE;
                world->player->position.y = r_jumper.y - r_player.h;
                world->player->is_on_block = true;
                jumpers[i].is_jumping = true;
            }
        }
    }
}

void update_jumper_positions(world_t *world, float delta) {

    for (int i = 0 ; i < jumpers_count ; i++) {
        jumper_t jumper = jumpers[i];
        if (jumper.position.y > WINDOW_HEIGHT) {
            jumpers[i].position.x = -100;
            jumpers[i].position.y = -100;
            jumpers[i].is_currently_uses = false;
            jumpers[i].is_jumping = false;
            jumpers[i].current_time_for_reset = 0;
        }

        if (jumpers[i].is_jumping && jumpers[i].current_time_for_reset < 0.5) {
            jumpers[i].current_time_for_reset += delta;
        } else {
            jumpers[i].is_jumping = false;
            jumpers[i].current_time_for_reset = 0;
        }

    }

}

void update_background(world_t *world, float delta) {

    if (background1_pos.y > WINDOW_HEIGHT) {
        background1_pos.y = background2_pos.y - background_h;
    }

    if (background2_pos.y > WINDOW_HEIGHT) {
        background2_pos.y = background1_pos.y - background_h;
    }
}

void render_background(SDL_Renderer *renderer, world_t *world) {

    float scaleX = (float)WINDOW_WIDTH / (float)t_background->w;
    float scaleY = (float)WINDOW_HEIGHT / (float)t_background->h;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    float new_img_w = scale * t_background->w * GAME_SCALE;
    float new_img_h = scale * t_background->h * GAME_SCALE;

    background_h = new_img_h;

    SDL_FRect background_rect1 = {background1_pos.x, background1_pos.y, new_img_w, new_img_h};
    SDL_RenderTexture(renderer, t_background, NULL, &background_rect1);

    SDL_FRect background_rect2 = {background2_pos.x, background2_pos.y, new_img_w, new_img_h};
    SDL_RenderTexture(renderer, t_background, NULL, &background_rect2);


}