#pragma once

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void LogInit(void);

void Log(LogLevel level, const char *format,...);

void LogClose(void);
