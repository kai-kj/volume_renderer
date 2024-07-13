#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>

typedef enum LogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} LogLevel;

typedef struct LogMessage {
    LogLevel level;
    time_t time;
    char *text;
} LogMessage;


#define debug(fmt, ...) logger_log(LOG_LEVEL_DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__)
#define info(fmt, ...) logger_log(LOG_LEVEL_INFO, fmt __VA_OPT__(, ) __VA_ARGS__)
#define warn(fmt, ...) logger_log(LOG_LEVEL_WARN, fmt __VA_OPT__(, ) __VA_ARGS__)
#define error(fmt, ...) logger_log(LOG_LEVEL_ERROR, fmt __VA_OPT__(, ) __VA_ARGS__)

void logger_create(int legnth);

void logger_destroy();

int logger_get_length();

LogMessage *logger_get_ith(int i);

void logger_log(LogLevel level, const char *fmt, ...);

#endif //LOGGER_H
