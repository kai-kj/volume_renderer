#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "external/glad.h"
#include "raylib.h"

#include "interface.h"

Config* cfg;
Font font;

static Vector2 draw_text(Vector2 pos, Color color, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int textLen = vsnprintf(NULL, 0, fmt, args);
    va_start(args, fmt);
    char* text = malloc(textLen + 1);
    vsnprintf(text, textLen + 1, fmt, args);
    va_end(args);

    DrawTextEx(font, text, pos, cfg->text.size, 0, color);
    Vector2 size = MeasureTextEx(font, text, cfg->text.size, 0);
    size.y += cfg->text.lineSpacing;

    printf("> %f, %f\n", size.x, size.y);

    free(text);
    return size;
}

static void draw_info_section() {
    Vector2 origin = (Vector2){
        cfg->sections.padding,
        cfg->sections.padding,
    };

    Vector2 size = (Vector2){
        cfg->sections.infoLogWidth,
        cfg->sections.infoHeight,
    };

    DrawRectangleLines(origin.x, origin.y, size.x, size.y, cfg->colors.fg1);

    Vector2 tPos = (Vector2){
        .x = origin.x + cfg->sections.padding,
        .y = origin.y + cfg->sections.padding,
    };

    BeginScissorMode(
        origin.x + cfg->sections.padding,
        origin.y + cfg->sections.padding,
        size.x - 2 * cfg->sections.padding,
        size.y - 2 * cfg->sections.padding
    );

    float fps = 1.0 / GetFrameTime();
    tPos.y += draw_text(tPos, cfg->colors.fg1, "fps: %02.2f", fps).y;

    char* glRenderer = (char*)glGetString(GL_RENDERER);
    tPos.y += draw_text(tPos, cfg->colors.fg1, "device: %s", glRenderer).y;

    BeginScissorMode(origin.x, origin.y, size.x, size.y);

    EndScissorMode();
}

static void draw_log_section() {
    Vector2 origin = (Vector2){
        cfg->sections.padding,
        cfg->sections.infoHeight + cfg->sections.padding * 2,
    };

    Vector2 size = (Vector2){
        cfg->sections.infoLogWidth,
        cfg->window.height - cfg->sections.infoHeight
            - 3 * cfg->sections.padding,
    };

    DrawRectangleLines(origin.x, origin.y, size.x, size.y, cfg->colors.fg1);

    Vector2 tPos = (Vector2){
        .x = origin.x + cfg->sections.padding,
        .y = origin.y + size.y - cfg->sections.padding - cfg->text.size,
    };

    BeginScissorMode(
        origin.x + cfg->sections.padding,
        origin.y + cfg->sections.padding,
        size.x - 2 * cfg->sections.padding,
        size.y - 2 * cfg->sections.padding
    );

    for (int i = 0; i < logger_get_length(cfg->logger); i++) {
        LogMessage* message = logger_get_ith(cfg->logger, i);
        if (!message || tPos.y < origin.y + cfg->sections.padding) break;

        tPos.x = origin.x + cfg->sections.padding;

        switch (message->level) {
            case LOG_LEVEL_DEBUG:
                tPos.x += draw_text(tPos, BLUE, "DEBUG ").x;
                break;
            case LOG_LEVEL_INFO:
                tPos.x += draw_text(tPos, GREEN, "INFO  ").x;
                break;
            case LOG_LEVEL_WARN:
                tPos.x += draw_text(tPos, ORANGE, "WARN  ").x;
                break;
            case LOG_LEVEL_ERROR:
                tPos.x += draw_text(tPos, RED, "ERROR ").x;
                break;
        }

        struct tm* timeinfo = localtime(&message->time);
        char timestamp[10];
        sprintf(
            timestamp,
            "%02d:%02d:%02d ",
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec
        );
        tPos.x += draw_text(tPos, cfg->colors.fg2, timestamp).x;

        tPos.y -= draw_text(tPos, cfg->colors.fg1, message->text).y;
    }

    EndScissorMode();
}

static void draw_preview_section() {
    Vector2 origin = (Vector2){
        cfg->sections.infoLogWidth + 2 * cfg->sections.padding,
        cfg->sections.padding,
    };

    Vector2 size = (Vector2){
        cfg->window.width - cfg->sections.infoLogWidth
            - 3 * cfg->sections.padding,
        cfg->window.height - 2 * cfg->sections.padding,
    };

    DrawRectangleLines(origin.x, origin.y, size.x, size.y, cfg->colors.fg1);
}

void interface_create(Config* config) {
    cfg = config;

    debug(cfg->logger, "creating interface");

    debug(cfg->logger, "creating window:");
    debug(cfg->logger, "- width: %d", cfg->window.width);
    debug(cfg->logger, "- height: %d", cfg->window.height);
    debug(cfg->logger, "- title: \"%s\"", cfg->window.title);

    InitWindow(cfg->window.width, cfg->window.height, cfg->window.title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(cfg->window.width, cfg->window.height);
    SetTargetFPS(60);

    debug(cfg->logger, "loading font: \"%s\"", cfg->text.font);
    font = LoadFontEx(cfg->text.font, cfg->text.size, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    if (!IsFontReady(font))
        error(cfg->logger, "failed to load font \"%s\"", cfg->text.font);
}

void interface_destroy() {
    debug(cfg->logger, "destroying interface");
    CloseWindow();
}

void interface_update() {
    if (IsWindowResized()) {
        cfg->window.width = GetScreenWidth();
        cfg->window.height = GetScreenHeight();
        debug(
            cfg->logger,
            "window resized: %dx%d",
            (int)cfg->window.width,
            (int)cfg->window.height
        );
    }

    BeginDrawing();
    ClearBackground(cfg->colors.bg);

    draw_info_section();
    draw_log_section();
    draw_preview_section();

    EndDrawing();
}

int interface_should_close() {
    return WindowShouldClose();
}