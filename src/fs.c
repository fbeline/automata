#include "fs.h"

#include <corecrt_share.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "log.h"

time_t FileTimeToTimeT(const FILETIME *ft) {
  ULARGE_INTEGER ull;
  ull.LowPart = ft->dwLowDateTime;
  ull.HighPart = ft->dwHighDateTime;
  return (time_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);
}

FileInfo **ListFiles(const char *dir, size_t *size) {
  *size = 0;
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;

  hFind = FindFirstFile(dir, &findFileData);
  if (hFind == INVALID_HANDLE_VALUE)
    return NULL;

  int count = 0;
  do {
    if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      count++;
  } while (FindNextFile(hFind, &findFileData) != 0);

  FileInfo **files = (FileInfo **)malloc(count * sizeof(FileInfo));
  FindClose(hFind);
  if (files == NULL) return NULL;

  hFind = FindFirstFile(dir, &findFileData);

  do {
    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      continue;

    files[*size] = (FileInfo *)malloc(sizeof(FileInfo));
    if (files[*size] == NULL) {
      FindClose(hFind);
      return NULL;
    }

    strcpy_s(files[*size]->cName, MAX_PATH, findFileData.cFileName);
    files[*size]->nFileSize = (unsigned long long)findFileData.nFileSizeHigh << 32 | findFileData.nFileSizeLow;
    files[*size]->tCreationTime = FileTimeToTimeT(&findFileData.ftLastWriteTime);

    *size += 1;
  } while (FindNextFile(hFind, &findFileData) != 0);

  FindClose(hFind);
  return files;
}

int AppDataPath(char *path) {
  size_t size;
  getenv_s(&size, path, MAX_PATH, "APPDATA");
  if (size == 0) {
    return 1;
  }

  strcat_s(path, MAX_PATH, "\\Automata");
  return 0;
}

void CreateDefaultLuaScript(void) {
  char path[MAX_PATH];
  if (AppDataPath(path) != 0) {
    return;
  }
  strcat_s(path, MAX_PATH, "\\default.lua");

  FILE *filePtr;
  filePtr = _fsopen(path, "r", _SH_DENYNO);
  if (filePtr != NULL) {
    fclose(filePtr);
    return;
  } 

  filePtr = _fsopen(path, "w", _SH_DENYNO);
  if (filePtr == NULL) {
    Log(LOG_ERROR, "Failed to create 'default.lua'");
    return;
  }

  fprintf(filePtr, "actions = {}");
  fclose(filePtr);
}
