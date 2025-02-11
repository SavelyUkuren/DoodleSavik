//
// Created by savely on 10.02.25.
//

#ifndef ASSETS_P_H
#define ASSETS_P_H

#include <unistd.h>
#include <sys/syslimits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

extern char *ASSETS_DIR;
extern char *BACKGROUND_TEXTURE;
extern char *PLAYER_LEFT_TEXTURE;
extern char *PLAYER_RIGHT_TEXTURE;
extern char *PLAYER_ODSKOK_LEFT_TEXTURE;
extern char *PLAYER_ODSKOK_RIGHT_TEXTURE;
extern char *GAME_TILES;

extern char *JUMP_SOUND;
extern char *JUMPER_SOUND;
extern char *FALL_SOUND;

void load_assets_paths(char *argv[]);
void free_assets();

#endif //ASSETS_P_H
