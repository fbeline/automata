#include "log.h"

#include <corecrt_share.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "fs.h"

#define LOG_MAX_LEN 1024

FILE *logFile = NULL;

void LogInit(void) {
  char path[MAX_PATH];
  if (AppDataPath(path) != 0) {
    Log(LOG_ERROR, "Error getting appdata path");
    exit(EXIT_FAILURE);
  }
  strcat_s(path, MAX_PATH, "//automata.log");
  logFile = _fsopen(path, "a", _SH_DENYNO);
  if (logFile == NULL) {
    Log(LOG_ERROR, "Error opening log file"); 
    exit(EXIT_FAILURE);
  }
}

void Log(LogLevel level, const char *format,...) {
  va_list args;
  char timestamp[20];
  char message[LOG_MAX_LEN];
  const char *levelStr;

  time_t rawtime;
  struct tm timeinfo;
  time(&rawtime);
  if (time(&rawtime) == -1 || localtime_s(&timeinfo, &rawtime) != 0) {
    perror("Error getting timestamp");
    return;
  }
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

  switch (level) {
    case LOG_INFO:
      levelStr = "INFO";
      break;
    case LOG_WARNING:
      levelStr = "WARNING";
      break;
    case LOG_ERROR:
      levelStr = "ERROR";
      break;
    default:
      levelStr = "UNKNOWN";
      break;
  }

  snprintf(message, sizeof(message), "[%s] [%s] ", timestamp, levelStr);

  va_start(args, format);
  vsnprintf(message + strlen(message), sizeof(message) - strlen(message), format, args);
  va_end(args);

  if (logFile != NULL) {
    fprintf(logFile, "%s\n", message);
    fflush(logFile);
  }
}

void LogClose(void) {
  if (logFile != NULL) {
    fclose(logFile);
  }
}
