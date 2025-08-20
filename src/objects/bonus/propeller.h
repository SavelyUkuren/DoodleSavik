#ifndef PROPELLER_H
#define PROPELLER_H

#include <SDL3/SDL.h>

#include "../../common/rect_t.h"
#include "../../common/point.h"

typedef enum propeller_state_t {
    PROPELLER_IDLE,
    PROPELLER_SPINNING
} propeller_state_t;

typedef struct propeller_t {
    rect_t rect;
} propeller_t;

void propeller_init(SDL_Renderer *renderer);
void render_propeller(SDL_Renderer *renderer, propeller_t *p, propeller_state_t state, float delta);

void propeller_destroy();

#endif //PROPELLER_H