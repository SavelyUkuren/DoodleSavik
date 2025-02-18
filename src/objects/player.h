#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#include "../common/point.h"
#include "../common/object_size.h"
#include "../common/vec2.h"
#include "../config.h"
#include "../assets_p.h"
#include "audio_controller.h"

typedef enum player_direction {
    NONE,
    LEFT,
    RIGHT
} player_direction;

typedef struct player_t {
    point position;
    object_size size;
    vec2 velocity;
    player_direction direction;
    int score;
    bool is_on_block;
} player_t;

void player_init(player_t *player, SDL_Renderer *renderer);
void player_logic(player_t *player, float delta);
void render_player(SDL_Renderer *renderer, player_t *player);
void destroy_player(player_t *player);

void player_jump(player_t *player, float with_force);

#endif // PLAYER_H
