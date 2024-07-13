#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

typedef struct Logger {
    int length;
    int next;
    LogMessage* hist;
} Logger;

Logger logger;

void logger_create(int length) {
    logger = (Logger){
        .length = length,
        .next = 0,
        .hist = malloc(sizeof(LogMessage) * length),
    };

    for (int i = 0; i < logger.length; i++)
        logger.hist[i] = (LogMessage){0, 0, NULL};
}

void logger_destroy() {
    for (int i = 0; i < logger.length; i++) {
        if (logger.hist[i].text) free(logger.hist[i].text);
    }

    logger = (Logger){
        .length = 0,
        .next = 0,
        .hist = NULL,
    };
}

int logger_get_length() {
    return logger.length;
}

LogMessage* logger_get_ith(int i) {
    if (i >= logger.length) return NULL;
    i = logger.next - 1 - i;
    if (i < 0) i = logger.length + i;
    return logger.hist[i].text && logger.hist[i].time ? &logger.hist[i] : NULL;
}

void logger_log(LogLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int message_len = vsnprintf(NULL, 0, fmt, args);
    va_start(args, fmt);
    char* message = malloc(message_len + 1);
    vsnprintf(message, message_len + 1, fmt, args);
    va_end(args);

    if (logger.hist[logger.next].text) free(logger.hist[logger.next].text);
    logger.hist[logger.next] = (LogMessage){level, time(NULL), message};
    logger.next++;
    if (logger.next >= logger.length) logger.next = 0;
}
