//
// Created by savely on 10.02.25.
//

#include "background.h"


point background1_pos;
point background2_pos;
point background3_pos;
float background_h;
SDL_Texture *t_background;

float _offset;

void init_background(SDL_Renderer *renderer) {
    t_background = IMG_LoadTexture(renderer, BACKGROUND_TEXTURE);
    if (!t_background) {
        printf("Failed to load '%s' texture\n", BACKGROUND_TEXTURE);
    }
    float scaleX = (float)WINDOW_WIDTH / (float)t_background->w;
    float scaleY = (float)WINDOW_HEIGHT / (float)t_background->h;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    background_h = scale * t_background->h * 1.3;
    background2_pos.y = -background_h;

    background3_pos.x = 0;
    background3_pos.y = WINDOW_HEIGHT;
}

void background_update(float delta) {
    if (background1_pos.y > WINDOW_HEIGHT) {
        background1_pos.y = background2_pos.y - background_h;
    }

    if (background2_pos.y > WINDOW_HEIGHT) {
        background2_pos.y = background1_pos.y - background_h;
    }

    if (_offset < 0) {

    }
}

void render_background(SDL_Renderer *renderer) {
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

    SDL_FRect background_rect3 = {background3_pos.x, background3_pos.y, new_img_w, new_img_h};
    SDL_RenderTexture(renderer, t_background, NULL, &background_rect3);
}

void background_offset(float offset, float delta) {
    background1_pos.y += offset;
    background2_pos.y += offset;
    _offset = offset;

    if (_offset < 0 && background3_pos.y > 0) {
        background3_pos.y += offset;
    }
}

void destroy_background() {
    SDL_DestroyTexture(t_background);
}
