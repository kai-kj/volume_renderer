#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "config.h"
#include "interface.h"
#include "logger.h"

typedef struct {
    Vector4 color;
    Vector4 properties;
} Voxel;

int main() {
    logger_create(256);

    InitWindow(cfg.windowSize.x, cfg.windowSize.y, "Volume renderer");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(cfg.windowSize.x, cfg.windowSize.y);

    SetTargetFPS(60);

    cfg.font
        = LoadFontEx("../assets/ubuntu.mono.ttf", cfg.textSize * 2, NULL, 0);

    info("starting app");

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            cfg.windowSize.x = GetScreenWidth();
            cfg.windowSize.y = GetScreenHeight();
            debug(
                "window resized: %dx%d",
                (int)cfg.windowSize.x,
                (int)cfg.windowSize.y
            );
        }

        update_interface();
    }

    CloseWindow();
    logger_destroy();
}
