#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

typedef struct Config {
    Vector2 windowSize;

    int textSize;
    int textSpacingV;
    int textSpacingH;

    Font font;

    int sectionPadding;
    int logSectionWidth;
} Config;

extern Config cfg;

#endif // CONFIG_H
