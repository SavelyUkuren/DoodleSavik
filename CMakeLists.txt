cmake_minimum_required(VERSION 3.20)

project(DoodleSavik LANGUAGES C)

include(FetchContent)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_image)

FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_mixer)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(DoodleSavik src/main.c
        src/objects/player.h src/objects/player.c
        src/common/point.h
        src/common/vec2.h
        src/common/object_size.h
        src/config.h
        src/objects/world.c
        src/objects/world.h
        src/common/point.c
        src/common/rect_t.h
        src/common/rect_t.c
        src/common/s_random.c
        src/common/s_random.h
        src/objects/assets_controller.c
        src/objects/assets_controller.h
        src/assets_p.h
        src/objects/blocks_controller.c
        src/objects/blocks_controller.h
        src/objects/background.c
        src/objects/background.h
        src/objects/audio_controller.c
        src/objects/audio_controller.h
        src/assets_p.c
)

target_link_libraries(DoodleSavik SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer)

if (UNIX)
    target_link_libraries(DoodleSavik m)
endif (UNIX)