//
// Created by savely on 10.02.25.
//

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL3/SDL.h>
#include <stdio.h>

#include "../libs/SDL_image.h"
#include "../common/point.h"
#include "../config.h"
#include "../assets_p.h"

void init_background(SDL_Renderer *renderer);

void background_update(float delta);
void render_background(SDL_Renderer *renderer);
void background_offset(float offset, float delta);

void destroy_background();

#endif //BACKGROUND_H
