#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>

#include "libs/SDL_image.h"

#include "objects/player.h"
#include "objects/world.h"
#include "config.h"
#include "objects/audio_controller.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

SDL_Texture *texture;

int is_running = 1;

int create_window();
int create_renderer();
void game_setup();
void game_loop();
void destroy_sdl();

float update_statistic_time = 0;
void render_fps(SDL_Renderer *renderer, float delta);
void render_frametime(SDL_Renderer *renderer, double duration);
void render_player_vel(SDL_Renderer *renderer, player_t *player);

player_t player;
world_t world;

int main()
{
    srand(2);

    if (create_window() != 0) return 1;
    if (create_renderer() != 0) return 1;

    game_setup();
    game_loop();

    destroy_sdl();
    return 0;
}

void game_setup() {
    audio_controller_init();

    player_init(&player, renderer);
    player.position.x = WINDOW_WIDTH / 2 - 64 / 2;
    player.position.y = WINDOW_HEIGHT / 2 - 64;
    player.size.w = 62 * GAME_SCALE;
    player.size.h = 60 * GAME_SCALE;
    player.velocity.dx = 0;
    player.velocity.dy = PLAYER_JUMP_FORCE;
    player.score = 0;

    world.player = &player;
    world_init(&world, renderer);

}

void game_loop() {

    Uint64 last_time = SDL_GetPerformanceCounter();
    float delta = 0;

    while (is_running) {
        Uint64 now = SDL_GetPerformanceCounter();
        Uint64 freq = SDL_GetPerformanceFrequency();

        delta = (float)(now - last_time) / freq;


        Uint64 start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                is_running = 0;
        }

        bool *key = SDL_GetKeyboardState(NULL);
        if (key[SDL_SCANCODE_D]) {
            player.direction = RIGHT;
        } else if (key[SDL_SCANCODE_A]) {
            player.direction = LEFT;
        } else if (key[SDL_SCANCODE_SPACE]) {

        } else {
            player.direction = NONE;
        }

        player_logic(&player, delta);
        world_logic(&world, delta);

        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        render_world(renderer, &world);
        render_player(renderer, &player);
        render_fps(renderer, delta);

        Uint64 end = SDL_GetPerformanceCounter();
        double duratuion = (double)(end - start) / SDL_GetPerformanceFrequency();
        render_frametime(renderer, duratuion);
        render_player_vel(renderer, &player);

        SDL_RenderPresent(renderer);
        last_time = now;
    }
}

int create_window() {
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL) {
        printf("Can't create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    return 0;
}

int create_renderer() {
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        printf("Can't create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if (ENABLE_VSYNC)
        SDL_SetRenderVSync(renderer, 1);
    return 0;
}

void destroy_sdl() {
    world_destroy(&world);
    destroy_player(&player);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render_fps(SDL_Renderer *renderer, float delta) {
    char fps_text[32];
    float fps = 1.0f / delta;  // Вычисляем FPS
    snprintf(fps_text, sizeof(fps_text), "FPS: %.2f", fps);

    SDL_RenderDebugText(renderer, 10, 10, fps_text);
}

void render_frametime(SDL_Renderer *renderer, double duration) {
    char frametime_text[32];
    snprintf(frametime_text, sizeof(frametime_text), "Frame time: %.3f ms", duration * 1000.0);
    SDL_RenderDebugText(renderer, 10, 30, frametime_text);
}

void render_player_vel(SDL_Renderer *renderer, player_t *player) {
    char vel_text[32];
    snprintf(vel_text, sizeof(vel_text), "Velocity: dx: %.2f, dy: %.2f", player->velocity.dx, player->velocity.dy);
    SDL_RenderDebugText(renderer, 10, 70, vel_text);
}