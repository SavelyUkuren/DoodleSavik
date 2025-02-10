//
// Created by savely on 10.02.25.
//

#include "jumpers_controller.h"

const int jumpers_count = 3;

jumper_t *jumpers;

void jumpers_init() {
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
}

void jumpers_update(float delta) {
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

void render_jumpers(SDL_Renderer *renderer) {
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

void jumpers_offset(float offset, float delta) {
    for (int i = 0; i < jumpers_count ; i++) {
        jumpers[i].position.y += offset;
    }
}

void player_on_jumper(player_t *player, float delta) {
    if (player->velocity.dy > 0) {
        rect_t r_player = {
            player->position.x,
            player->position.y,
            player->size.w,
            player->size.h
        };

        for (int i = 0 ; i < jumpers_count; i++) {

            rect_t r_jumper = {
                jumpers[i].position.x,
                jumpers[i].position.y,
                jumpers[i].size.w,
                jumpers[i].size.h
            };

            if (is_on_top(&r_player, &r_jumper, player->velocity.dy)) {
                player->velocity.dy = PLAYER_ON_JUMPER_FORCE;
                player->position.y = r_jumper.y - r_player.h;
                player->is_on_block = true;
                jumpers[i].is_jumping = true;
            }
        }
    }
}

void destroy_jumpers() {
    free(jumpers);
}
