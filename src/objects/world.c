//
// Created by savely on 31.01.25.
//

#include "world.h"

float world_offset_y = 0;
SDL_Texture *game_tiles;

bool is_game_over = false;

void world_init(world_t *world, SDL_Renderer *renderer) {

    game_tiles = IMG_LoadTexture(renderer, GAME_TILES);
    if (!game_tiles) {
        printf("Failed to load '%s' texture\n", GAME_TILES);
    }

    init_background(renderer);
    blocks_init();
    jumpers_init();

}

void world_logic(world_t *world, float delta) {
    background_update(delta);
    update_camera(world, delta);
    blocks_update(delta);
    jumpers_update(delta);
    player_on_block(world->player, delta);
    player_on_jumper(world->player, delta);

    if (world->player->position.y + world->player->size.h > WINDOW_HEIGHT
        && !is_game_over) {
        is_game_over = true;
        play_fall_sound();
    }
}

void render_world(SDL_Renderer *renderer, world_t *world) {

    render_background(renderer);
    render_blocks(renderer);
    render_jumpers(renderer);

}

void world_destroy(world_t *world) {

    SDL_DestroyTexture(game_tiles);
    destroy_blocks();
    destroy_jumpers();
    destroy_background();
}

bool is_on_top(rect_t *rect1, rect_t *rect2, float velocity_y) {
    return (rect1->x + rect1->w > rect2->x) &&
           (rect1->x < rect2->x + rect2->w) &&
           (rect1->y + rect1->h >= rect2->y) &&
           (rect1->y + rect1->h - velocity_y < rect2->y) &&
           (velocity_y > 0);
}

void update_camera(world_t *world, float delta) {
    world_offset_y = -world->player->velocity.dy * 100 * delta;

    if (world->player->position.y < WINDOW_HEIGHT / 3) {
        world->player->position.y = WINDOW_HEIGHT / 3;

        blocks_offset(world_offset_y, delta);
        jumpers_offset(world_offset_y, delta);
        background_offset(world_offset_y, delta);

        world->player->score += (int)roundf(100.0f * delta);
    }

    if (world->player->position.y + world->player->size.h > WINDOW_HEIGHT &&
        world->player->position.x < WINDOW_HEIGHT * 2) {
        blocks_offset(world_offset_y, delta);
        jumpers_offset(world_offset_y, delta);
        background_offset(world_offset_y, delta);
    }

}
