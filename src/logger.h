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
    char* text;
} LogMessage;

typedef struct Logger Logger;

#define debug(logger, fmt, ...)                                                \
    logger_log(logger, LOG_LEVEL_DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__)
#define info(logger, fmt, ...)                                                 \
    logger_log(logger, LOG_LEVEL_INFO, fmt __VA_OPT__(, ) __VA_ARGS__)
#define warn(logger, fmt, ...)                                                 \
    logger_log(logger, LOG_LEVEL_WARN, fmt __VA_OPT__(, ) __VA_ARGS__)
#define error(logger, fmt, ...)                                                \
    logger_log(logger, LOG_LEVEL_ERROR, fmt __VA_OPT__(, ) __VA_ARGS__)

Logger* logger_create(int legnth);

void logger_destroy(Logger* logger);

int logger_get_length(Logger* logger);

LogMessage* logger_get_ith(Logger* logger, int i);

void logger_log(Logger* logger, LogLevel level, const char* fmt, ...);

#endif // LOGGER_H
