#include "propeller.h"

#include <SDL3_image/SDL_image.h>

#include "../../assets_p.h"

SDL_Texture *propeller_texture;

float frame = 0;

void propeller_init(SDL_Renderer *renderer) {
    propeller_texture = IMG_LoadTexture(renderer, PROPELLER_TEXTURE);
    if (!propeller_texture) 
        printf("Failed to load '%s' left\n", PLAYER_LEFT_TEXTURE);
}

void render_propeller(SDL_Renderer *renderer,
     propeller_t *p, propeller_state_t state,
    float delta) {

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (state == PROPELLER_IDLE) {
        SDL_FRect src_r = {0, 0, 64, 64};
        SDL_FRect r = {p->rect.x, p->rect.y, p->rect.w, p->rect.h};
        SDL_RenderTexture(renderer, propeller_texture, &src_r, &r);
    } else if (state == PROPELLER_SPINNING) {
        
        SDL_FRect src_r = {0, 0, 64, 64};
        if ((int)frame == 0) {
            src_r.x = 64;
        } else if ((int)frame == 1) {
            src_r.x = 0;
            src_r.y = 64;
        } else if ((int)frame == 2) {
            src_r.x = 64;
            src_r.y = 64;
        }
        SDL_FRect r = {p->rect.x, p->rect.y, p->rect.w, p->rect.h};
        SDL_RenderTexture(renderer, propeller_texture, &src_r, &r);
    }

    frame += delta * 20;
    if (frame > 3) 
        frame -= 3;
    
}