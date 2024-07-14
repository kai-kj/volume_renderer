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

    DrawTextEx(font, text, pos, cfg->textSize, cfg->textSpacingX, color);
    Vector2 size = MeasureTextEx(font, text, cfg->textSize, cfg->textSpacingX);
    size.y += cfg->textSpacingY;

    free(text);
    return size;
}

static void draw_info_section() {
    Vector2 origin = (Vector2){
        cfg->sectionPadding,
        cfg->sectionPadding,
    };

    Vector2 size = (Vector2){
        cfg->infoLogSectionWidth,
        cfg->infoSectionHeight,
    };

    DrawRectangleLines(origin.x, origin.y, size.x, size.y, WHITE);

    Vector2 textPos = (Vector2){
        .x = origin.x + cfg->sectionPadding,
        .y = origin.y + cfg->sectionPadding,
    };

    BeginScissorMode(
        origin.x + cfg->sectionPadding,
        origin.y + cfg->sectionPadding,
        size.x - 2 * cfg->sectionPadding,
        size.y - 2 * cfg->sectionPadding
    );

    float fps = 1.0 / GetFrameTime();
    textPos.y += draw_text(textPos, WHITE, "fps: %02.2f", fps).y;

    char* glRenderer = (char*)glGetString(GL_RENDERER);
    textPos.y += draw_text(textPos, WHITE, "device: %s", glRenderer).y;

    BeginScissorMode(origin.x, origin.y, size.x, size.y);

    EndScissorMode();
}

static void draw_log_section() {
    Vector2 origin = (Vector2){
        cfg->sectionPadding,
        cfg->infoSectionHeight + cfg->sectionPadding * 2,
    };

    Vector2 size = (Vector2){
        cfg->infoLogSectionWidth,
        cfg->windowSizeY - cfg->infoSectionHeight - 3 * cfg->sectionPadding,
    };

    DrawRectangleLines(origin.x, origin.y, size.x, size.y, WHITE);

    Vector2 textPos = (Vector2){
        .x = origin.x + cfg->sectionPadding,
        .y = origin.y + size.y - cfg->sectionPadding - cfg->textSize,
    };

    BeginScissorMode(
        origin.x + cfg->sectionPadding,
        origin.y + cfg->sectionPadding,
        size.x - 2 * cfg->sectionPadding,
        size.y - 2 * cfg->sectionPadding
    );

    for (int i = 0; i < logger_get_length(cfg->logger); i++) {
        LogMessage* message = logger_get_ith(cfg->logger, i);
        if (!message || textPos.y < origin.y + cfg->sectionPadding) break;

        textPos.x = origin.x + cfg->sectionPadding;

        switch (message->level) {
            case LOG_LEVEL_DEBUG:
                textPos.x += draw_text(textPos, BLUE, "DEBUG ").x;
                break;
            case LOG_LEVEL_INFO:
                textPos.x += draw_text(textPos, GREEN, "INFO  ").x;
                break;
            case LOG_LEVEL_WARN:
                textPos.x += draw_text(textPos, ORANGE, "WARN  ").x;
                break;
            case LOG_LEVEL_ERROR:
                textPos.x += draw_text(textPos, RED, "ERROR ").x;
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
        textPos.x += draw_text(textPos, GRAY, timestamp).x;

        textPos.y -= draw_text(textPos, WHITE, message->text).y;
    }

    EndScissorMode();
}

static void draw_preview_section() {
    Vector2 origin = (Vector2){
        cfg->infoLogSectionWidth + 2 * cfg->sectionPadding,
        cfg->sectionPadding,
    };

    Vector2 size = (Vector2){
        cfg->windowSizeX - cfg->infoLogSectionWidth - 3 * cfg->sectionPadding,
        cfg->windowSizeY - 2 * cfg->sectionPadding,
    };

    DrawRectangleLines(origin.x, origin.y, size.x, size.y, WHITE);
}

void interface_create(Config* config) {
    cfg = config;

    debug(cfg->logger, "creating interface");

    debug(cfg->logger, "creating window:");
    debug(cfg->logger, "- width: %d", cfg->windowSizeX);
    debug(cfg->logger, "- height: %d", cfg->windowSizeY);
    debug(cfg->logger, "- title: \"%s\"", cfg->windowTitle);

    InitWindow(cfg->windowSizeX, cfg->windowSizeY, cfg->windowTitle);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(cfg->windowSizeX, cfg->windowSizeY);
    SetTargetFPS(60);

    debug(cfg->logger, "loading font: \"%s\"", cfg->fontFile);
    font = LoadFontEx(cfg->fontFile, cfg->textSize, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
}

void interface_destroy() {
    debug(cfg->logger, "destroying interface");
    CloseWindow();
}

void interface_update() {
    if (IsWindowResized()) {
        cfg->windowSizeX = GetScreenWidth();
        cfg->windowSizeY = GetScreenHeight();
        debug(
            cfg->logger,
            "window resized: %dx%d",
            (int)cfg->windowSizeX,
            (int)cfg->windowSizeY
        );
    }

    BeginDrawing();
    ClearBackground(BLACK);

    draw_info_section();
    draw_log_section();
    draw_preview_section();

    EndDrawing();
}

int interface_should_close() {
    return WindowShouldClose();
}