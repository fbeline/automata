#pragma once

#include <time.h>

#define MAX_PATH 260

typedef struct fileInfo {
  char cName[MAX_PATH];
  time_t tCreationTime;
  unsigned long long nFileSize;
} FileInfo;

FileInfo** ListFiles(const char* dir, size_t *size);

int AppDataPath(char *path);

int CreateDefaultLuaScript(void);
