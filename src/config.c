#include <stdlib.h>
#include <string.h>

#include "ini.h"

#include "config.h"

#define match(s, n) (strcmp(section, s) == 0 && strcmp(name, n) == 0)

static Color to_color(const char* value) {
    long hex = strtol(value, NULL, 0);
    return (Color){
        .r = (unsigned char)((hex >> 16) & 0xFF),
        .g = (unsigned char)((hex >> 8) & 0xFF),
        .b = (unsigned char)((hex >> 0) & 0xFF),
        .a = 0xFF,
    };
}

static int handler(
    void* arg,
    const char* section,
    const char* name,
    const char* value
) {
    Config* config = (Config*)arg;

    debug(config->logger, "- %s.%s = %s", section, name, value);

    if (match("colors", "bg")) {
        config->colors.bg = to_color(value);
    } else if (match("colors", "fg1")) {
        config->colors.fg1 = to_color(value);
    } else if (match("colors", "fg2")) {
        config->colors.fg2 = to_color(value);
    } else if (match("window", "width")) {
        config->window.width = atoi(value);
    } else if (match("window", "height")) {
        config->window.height = atoi(value);
    } else if (match("window", "title")) {
        config->window.title = strdup(value);
    } else if (match("text", "font")) {
        config->text.font = strdup(value);
    } else if (match("text", "size")) {
        config->text.size = atoi(value);
    } else if (match("text", "lineSpacing")) {
        config->text.lineSpacing = atoi(value);
    } else if (match("sections", "padding")) {
        config->sections.padding = atoi(value);
    } else if (match("sections", "infoHeight")) {
        config->sections.infoHeight = atoi(value);
    } else if (match("sections", "infoLogWidth")) {
        config->sections.infoLogWidth = atoi(value);
    }

    return 1;
}

Config* config_create(Logger* logger, char* fileName) {
    Config* config = malloc(sizeof *config);
    config->logger = logger;

    info(config->logger, "reading config file \"%s\"", fileName);

    if (ini_parse(fileName, handler, config) < 0)
        error(config->logger, "failed to read config file \"%s\"", fileName);

    return config;
}

void config_destroy(Config* config) {
    if (!config) return;

    debug(config->logger, "destroying config");
    free(config);
}
