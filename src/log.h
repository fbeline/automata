#pragma once

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void LogInit(const char *filename);

void Log(LogLevel level, const char *format,...);

void LogClose(void);
