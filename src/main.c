#include <stdio.h>
#include <stdlib.h>

#include "interface/interface.h"
#include "misc/config.h"
#include "misc/logger.h"

int main() {
    Logger* logger = logger_create(256);
    Config* config = config_create(logger, "data/config.ini");

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
