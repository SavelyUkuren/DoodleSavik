//
// Created by Савелий Никулин on 11.02.2025.
//

#include "assets_p.h"

char *ASSETS_DIR;
char *BACKGROUND_TEXTURE;
char *PLAYER_LEFT_TEXTURE;
char *PLAYER_RIGHT_TEXTURE;
char *PLAYER_ODSKOK_LEFT_TEXTURE;
char *PLAYER_ODSKOK_RIGHT_TEXTURE;
char *GAME_TILES;

char *JUMP_SOUND;
char *JUMPER_SOUND;
char *FALL_SOUND;

void load_assets_paths(char *argv[]) {

    char path[PATH_MAX];
    strncpy(path, argv[0], sizeof(path));
    char *dir = dirname(path);

    BACKGROUND_TEXTURE = malloc(PATH_MAX);
    snprintf(BACKGROUND_TEXTURE, PATH_MAX, "%s/%s", dir, "assets/bck@2x.png");

    PLAYER_LEFT_TEXTURE = malloc(PATH_MAX);
    snprintf(PLAYER_LEFT_TEXTURE, PATH_MAX, "%s/%s", dir, "assets/blue-lik-left@2x.png");

    PLAYER_RIGHT_TEXTURE = malloc(PATH_MAX);
    snprintf(PLAYER_RIGHT_TEXTURE, PATH_MAX, "%s/%s", dir, "assets/blue-lik-right@2x.png");

    PLAYER_ODSKOK_LEFT_TEXTURE = malloc(PATH_MAX);
    snprintf(PLAYER_ODSKOK_LEFT_TEXTURE, PATH_MAX, "%s/%s", dir, "assets/blue-lik-left-odskok@2x.png");

    PLAYER_ODSKOK_RIGHT_TEXTURE = malloc(PATH_MAX);
    snprintf(PLAYER_ODSKOK_RIGHT_TEXTURE, PATH_MAX, "%s/%s", dir, "assets/blue-lik-right-odskok@2x.png");

    GAME_TILES = malloc(PATH_MAX);
    snprintf(GAME_TILES, PATH_MAX, "%s/%s", dir, "assets/game-tiles@2x.png");

    JUMP_SOUND = malloc(PATH_MAX);
    snprintf(JUMP_SOUND, PATH_MAX, "%s/%s", dir, "assets/jump.wav");

    JUMPER_SOUND = malloc(PATH_MAX);
    snprintf(JUMPER_SOUND, PATH_MAX, "%s/%s", dir, "assets/feder.wav");

    FALL_SOUND = malloc(PATH_MAX);
    snprintf(FALL_SOUND, PATH_MAX, "%s/%s", dir, "assets/pada.wav");

    ASSETS_DIR = "assets/";
}

void free_assets() {
    free(BACKGROUND_TEXTURE);
    free(PLAYER_LEFT_TEXTURE);
    free(PLAYER_RIGHT_TEXTURE);
    free(PLAYER_ODSKOK_LEFT_TEXTURE);
    free(PLAYER_ODSKOK_RIGHT_TEXTURE);
    free(GAME_TILES);
    free(JUMP_SOUND);
    free(JUMPER_SOUND);
    free(FALL_SOUND);
}