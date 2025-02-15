//
// Created by Савелий Никулин on 10.02.2025.
//

#include "audio_controller.h"

Mix_Chunk *jump_sound;
Mix_Chunk *jumper_sound;
Mix_Chunk *fall_sound;

void audio_controller_init() {
    SDL_AudioSpec desired;
    desired.freq = 48000;
    desired.channels = 2;
    desired.format = SDL_AUDIO_S16;

    if (!Mix_OpenAudio(0, &desired)) {
        printf("Can't open audio: %s\n", SDL_GetError());
        return;
    }

    jump_sound = Mix_LoadWAV(JUMP_SOUND);
    if (!jump_sound) {
        printf("Error. Ca't open '%s'\n", JUMP_SOUND);
        return;
    }

    jumper_sound = Mix_LoadWAV(JUMPER_SOUND);
    if (!jumper_sound) {
        printf("Error. Ca't open '%s'\n", JUMPER_SOUND);
        return;
    }

    fall_sound = Mix_LoadWAV(FALL_SOUND);
    if (!fall_sound) {
        printf("Error. Ca't open '%s'\n", FALL_SOUND);
    }

}

void play_jump_sound() {
    if (SOUND_ON)
        Mix_PlayChannel(-1, jump_sound, 0);
}

void play_jumper_sound() {
    if (SOUND_ON)
        Mix_PlayChannel(-1, jumper_sound, 0);
}

void play_fall_sound() {
    if (SOUND_ON)
        Mix_PlayChannel(-1, fall_sound, 0);
}

void destroy_audio() {
    Mix_FreeChunk(jump_sound);
    Mix_CloseAudio();
}