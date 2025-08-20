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
#include "bonus/propeller.h"


typedef enum bonus_t {
    BONUS_NONE,
    BONUS_PROPELLER
} bonus_t;

typedef enum player_direction {
    DIR_NONE,
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
    bonus_t current_bonus;
} player_t;

void player_init(player_t *player, SDL_Renderer *renderer);
void player_logic(player_t *player, float delta);
void render_player(SDL_Renderer *renderer, player_t *player);
void destroy_player(player_t *player);

// void add_propeller(player_t *player);

void player_jump(player_t *player, float with_force);

#endif // PLAYER_H
