//
// Created by Савелий Никулин on 10.02.2025.
//
#define MINIAUDIO_IMPLEMENTATION

#include "audio_controller.h"
#include "../lib/miniaudio.h"

ma_engine engine;
ma_sound jump_sound, jumper_sound, fall_sound, propeller_sound;

void audio_controller_init() {

    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        printf("Failed to initialize audio engine\n");
        return;
    }

    if (ma_sound_init_from_file(&engine, JUMP_SOUND, MA_SOUND_FLAG_DECODE, NULL, NULL, &jump_sound) != MA_SUCCESS) {
        printf("Error. Ca't open '%s'\n", JUMP_SOUND);
        return;
    }

    if (ma_sound_init_from_file(&engine, JUMPER_SOUND, MA_SOUND_FLAG_DECODE, NULL, NULL, &jumper_sound) != MA_SUCCESS) {
        printf("Error. Ca't open '%s'\n", JUMPER_SOUND);
        return;
    }

    if (ma_sound_init_from_file(&engine, FALL_SOUND, MA_SOUND_FLAG_DECODE, NULL, NULL, &fall_sound) != MA_SUCCESS) {
        printf("Error. Ca't open '%s'\n", FALL_SOUND);
        return;
    }

    if (ma_sound_init_from_file(&engine, PROPELLER_SOUND, MA_SOUND_FLAG_DECODE, NULL, NULL, &propeller_sound) != MA_SUCCESS) {
        printf("Error. Ca't open '%s'\n", PROPELLER_SOUND);
        return;
    }

    ma_sound_set_volume(&jump_sound, VOLUME);
    ma_sound_set_volume(&jumper_sound, VOLUME);
    ma_sound_set_volume(&fall_sound, VOLUME);
    ma_sound_set_volume(&propeller_sound, VOLUME);

}

void play_jump_sound() {
    if (SOUND_ON)
        ma_sound_start(&jump_sound);
}

void play_jumper_sound() {
    if (SOUND_ON)
        ma_sound_start(&jumper_sound);
}

void play_fall_sound() {
    if (SOUND_ON)
        ma_sound_start(&fall_sound);
}

void play_propeller_sound() {
    if (SOUND_ON)
        ma_sound_start(&propeller_sound);
}

void destroy_audio() {
    ma_sound_uninit(&jump_sound);
    ma_sound_uninit(&jumper_sound);
    ma_sound_uninit(&fall_sound);
    ma_sound_uninit(&propeller_sound);
}