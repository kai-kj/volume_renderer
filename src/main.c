#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "interface.h"
#include "logger.h"

int main() {
    Logger* logger = logger_create(256);
    Config* config = config_create_default(logger);

    interface_create(config);

    info(logger, "starting app");

    while (!interface_should_close()) {
        interface_update();
    }

    info(logger, "stopping app");

    interface_destroy();
    config_destroy(config);
    logger_destroy(logger);
}
