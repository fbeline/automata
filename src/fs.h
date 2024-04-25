#pragma once

#include <time.h>

#define MAX_PATH_LENGTH 256

typedef struct fileInfo {
  char cName[MAX_PATH_LENGTH];
  time_t tCreationTime;
  unsigned long long nFileSize;
} FileInfo;

FileInfo** ListFiles(const char* dir, int *size);

void AppDataPath(char path[MAX_PATH_LENGTH]);
