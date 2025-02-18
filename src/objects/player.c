#include "player.h"

char score_text[32];

SDL_Texture *player_texture_left;
SDL_Texture *player_texture_right;
SDL_Texture *player_texture_odskok_left;
SDL_Texture *player_texture_odskok_right;

player_direction last_direction = LEFT;
float player_odskok_current_time = 0;
bool is_odskok = false;

void player_init(player_t *player, SDL_Renderer *renderer) {

    snprintf(score_text, sizeof(score_text), "Score: %d", player->score);

    player_texture_left = IMG_LoadTexture(renderer, PLAYER_LEFT_TEXTURE);
    if (!player_texture_left) {
        printf("Failed to load '%s' left\n", PLAYER_LEFT_TEXTURE);
    }

    player_texture_right = IMG_LoadTexture(renderer, PLAYER_RIGHT_TEXTURE);
    if (!player_texture_right) {
        printf("Failed to load '%s' right\n", PLAYER_RIGHT_TEXTURE);
    }

    player_texture_odskok_left = IMG_LoadTexture(renderer, PLAYER_ODSKOK_LEFT_TEXTURE);
    if (!player_texture_odskok_left) {
        printf("Failed to load '%s' right\n", PLAYER_ODSKOK_LEFT_TEXTURE);
    }

    player_texture_odskok_right = IMG_LoadTexture(renderer, PLAYER_ODSKOK_RIGHT_TEXTURE);
    if (!player_texture_odskok_right) {
        printf("Failed to load '%s' right\n", PLAYER_ODSKOK_RIGHT_TEXTURE);
    }
}

void player_logic(player_t *player, float delta) {

    switch (player->direction) {
    case LEFT:
        player->velocity.dx += -PLAYER_ACCELERATION * delta;
        last_direction = LEFT;
        break;
    case RIGHT:
        player->velocity.dx += PLAYER_ACCELERATION * delta;
        last_direction = RIGHT;
        break;
    case NONE:
        if (player->velocity.dx < 0) {
            player->velocity.dx += PLAYER_ACCELERATION * delta;
        }
        if (player->velocity.dx > 0) {
            player->velocity.dx += -PLAYER_ACCELERATION * delta;
        }
        break;
    }
    player->velocity.dy += (float)GRAVITY * delta;
    if (player->velocity.dy > 10) {
        player->velocity.dy = 10;
    }

    if (player->velocity.dx >= PLAYER_MAX_SPEED) {
        player->velocity.dx = PLAYER_MAX_SPEED;
    }
    if (player->velocity.dx <= -PLAYER_MAX_SPEED) {
        player->velocity.dx = -PLAYER_MAX_SPEED;
    }

    player->position.y += player->velocity.dy * 60 * delta;
    player->position.x += player->velocity.dx * delta;

    if (player->position.x + player->size.w / 2 + 2 > WINDOW_WIDTH) {
        player->position.x = -player->size.w / 2 + 4;
    }
    if (player->position.x + player->size.w / 2 < 0) {
        player->position.x = WINDOW_WIDTH - player->size.w / 2 - 4;
    }

    snprintf(score_text, sizeof(score_text), "Score: %d", player->score);

    if (player->is_on_block) {
        is_odskok = true;
    }

    if (is_odskok && player_odskok_current_time < 0.2) {
        player_odskok_current_time += delta;
    } else {
        player_odskok_current_time = 0;
        is_odskok = false;
    }
}

void render_player(SDL_Renderer *renderer, player_t *player) {

    SDL_FRect r = {player->position.x,
                   player->position.y,
                   player->size.w,
                   player->size.h};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderFillRect(renderer, &r);

    if (is_odskok) {
        switch (last_direction) {
            case LEFT:
                SDL_RenderTexture(renderer, player_texture_odskok_left, NULL, &r);
            break;
            case RIGHT:
                SDL_RenderTexture(renderer, player_texture_odskok_right, NULL, &r);
            break;
            case NONE:
                break;
        }
    } else {
        switch (last_direction) {
            case LEFT:
                SDL_RenderTexture(renderer, player_texture_left, NULL, &r);
            break;
            case RIGHT:
                SDL_RenderTexture(renderer, player_texture_right, NULL, &r);
            break;
            case NONE:
                break;
        }
    }


    SDL_RenderDebugText(renderer, 10, 50, score_text);
}

void player_jump(player_t *player, float with_force) {
    player->velocity.dy = with_force;
    player->is_on_block = true;
}

void destroy_player(player_t *player) {
    SDL_DestroyTexture(player_texture_left);
    SDL_DestroyTexture(player_texture_right);
    SDL_DestroyTexture(player_texture_odskok_left);
    SDL_DestroyTexture(player_texture_odskok_right);



}