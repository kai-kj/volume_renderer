#ifndef CONFIG_H
#define CONFIG_H

#include "misc/logger.h"
#include "raylib.h"

typedef struct Config {
    Logger* logger;

    struct {
        Color bg;
        Color fg1;
        Color fg2;
    } colors;

    struct {
        int width;
        int height;
        char* title;
    } window;

    struct {
        char* font;
        int size;
        int lineSpacing;
    } text;

    struct {
        int padding;
        int infoHeight;
        int infoLogWidth;
    } sections;
} Config;

Config* config_create(Logger* logger, char* fileName);

void config_destroy(Config* config);

#endif // CONFIG_H
