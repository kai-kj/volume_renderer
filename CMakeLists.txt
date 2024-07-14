cmake_minimum_required(VERSION 3.5)
project(volume_renderer C)

set(CMAKE_C_STANDARD 11)

set(PLATFORM "Desktop" CACHE STRING "" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/raylib)

add_library(
        inih
        external/inih/ini.c
)

target_include_directories(
        inih PUBLIC
        external/inih/
)

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

target_link_libraries(volume_renderer raylib inih)

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