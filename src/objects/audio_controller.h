//
// Created by Савелий Никулин on 10.02.2025.
//

#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include <SDL3_mixer/SDL_mixer.h>

#include "../assets_p.h"
#include "../config.h"

void audio_controller_init();

void play_jump_sound();
void play_jumper_sound();
void play_fall_sound();

void destroy_audio();
#endif //AUDIO_CONTROLLER_H
