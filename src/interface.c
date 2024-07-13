#include <stdio.h>

#include "raylib.h"

#include "config.h"
#include "interface.h"
#include "logger.h"

static void draw_text(char* text, Vector2 pos, Color color) {
    DrawTextEx(cfg.font, text, pos, cfg.textSize, cfg.textSpacingH, color);
}

static void draw_log_section() {
    DrawRectangleLines(
        cfg.sectionPadding,
        cfg.sectionPadding,
        cfg.logSectionWidth,
        cfg.windowSize.y - 2 * cfg.sectionPadding,
        WHITE
    );

    Vector2 textPos = (Vector2){
        .x = cfg.sectionPadding + cfg.textSpacingV,
        .y = cfg.windowSize.y - cfg.sectionPadding - cfg.textSpacingV
           - cfg.textSize,
    };

    BeginScissorMode(
        cfg.sectionPadding,
        cfg.sectionPadding,
        cfg.logSectionWidth,
        cfg.windowSize.y - 2 * cfg.sectionPadding
    );

    for (int i = 0; i < logger_get_length(); i++) {
        LogMessage* message = logger_get_ith(i);
        if (!message || textPos.y < cfg.sectionPadding + cfg.textSpacingV)
            break;

        textPos.x = cfg.sectionPadding + cfg.textSpacingV;

        switch (message->level) {
            case LOG_LEVEL_DEBUG: draw_text("DEBUG", textPos, BLUE); break;
            case LOG_LEVEL_INFO: draw_text("INFO", textPos, GREEN); break;
            case LOG_LEVEL_WARN: draw_text("WARN", textPos, ORANGE); break;
            case LOG_LEVEL_ERROR: draw_text("ERROR", textPos, RED); break;
        }
        textPos.x += 50;

        struct tm* timeinfo = localtime(&message->time);
        char timestamp[9];
        sprintf(
            timestamp,
            "%02d:%02d:%02d",
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec
        );
        DrawTextEx(cfg.font, timestamp, textPos, cfg.textSize, 0, GRAY);
        textPos.x += 75;

        draw_text(message->text, textPos, WHITE);
        textPos.y -= cfg.textSpacingV + cfg.textSize;
    }

    EndScissorMode();
}

static void draw_preview_section() {
    DrawRectangleLines(
        cfg.logSectionWidth + 2 * cfg.sectionPadding,
        cfg.sectionPadding,
        cfg.windowSize.x - cfg.logSectionWidth - 3 * cfg.sectionPadding,
        cfg.windowSize.y - 2 * cfg.sectionPadding,
        WHITE
    );
}

void update_interface() {
    BeginDrawing();
    ClearBackground(BLACK);

    draw_log_section();
    draw_preview_section();

    EndDrawing();
}
