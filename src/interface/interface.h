#ifndef INTERFACE_H
#define INTERFACE_H

#include "misc/config.h"

void interface_create(Config* config);

void interface_destroy();

void interface_update();

int interface_should_close();

#endif // INTERFACE_H
