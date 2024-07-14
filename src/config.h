#ifndef CONFIG_H
#define CONFIG_H

#include "logger.h"

typedef struct Config {
    Logger* logger;

    unsigned int windowSizeX;
    unsigned int windowSizeY;
    char* windowTitle;
    char* fontFile;

    int textSize;
    int textSpacingX;
    int textSpacingY;

    unsigned int sectionPadding;
    unsigned int infoSectionHeight;
    unsigned int infoLogSectionWidth;
} Config;

Config* config_create_default(Logger* logger);

void config_destroy(Config* config);

#endif // CONFIG_H
