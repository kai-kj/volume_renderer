cmake_minimum_required(VERSION 3.2)
project(volume_renderer C)

set(CMAKE_C_STANDARD 11)

set(PLATFORM "Desktop" CACHE STRING "" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/raylib)

add_executable(
        volume_renderer
        src/main.c
        src/logger.c
        src/interface.c
        src/renderer.c
        src/config.c
)

target_include_directories(
        volume_renderer PRIVATE
        external/raylib/src
        src/
)

target_link_libraries(volume_renderer raylib)

target_compile_options(
        volume_renderer PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unused-variable
        -Wno-unused-parameter
        -Wno-missing-braces
        -Wno-unused-function
)

add_custom_command(
        TARGET volume_renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
)