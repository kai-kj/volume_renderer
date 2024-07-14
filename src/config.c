#include "config.h"

#include <stdlib.h>

Config* config_create_default(Logger* logger) {
    Config* config = malloc(sizeof *config);

    *config = (Config){
        .logger = logger,

        .windowSizeX = 1000,
        .windowSizeY = 600,
        .windowTitle = "Volumn renderer",
        .fontFile = "assets/UbuntuMono.ttf",

        .textSize = 14,
        .textSpacingX = 0,
        .textSpacingY = 2,

        .sectionPadding = 10,
        .infoSectionHeight = 200,
        .infoLogSectionWidth = 400,
    };

    debug(config->logger, "creating default config");
    return config;
}

void config_destroy(Config* config) {
    if (!config) return;

    debug(config->logger, "destroying config");
    free(config);
}