cmake_minimum_required(VERSION 3.16)

project(DoodleSavik LANGUAGES C)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

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
)

target_link_libraries(DoodleSavik SDL3::SDL3 SDL3_image::SDL3_image)

if (UNIX)
    target_link_libraries(DoodleSavik m)
endif (UNIX)