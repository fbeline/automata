#include "fs.h"

#include <stdio.h>
#include <windows.h>
#include <string.h>

time_t FileTimeToTimeT(const FILETIME* ft) {
  ULARGE_INTEGER ull;
  ull.LowPart = ft->dwLowDateTime;
  ull.HighPart = ft->dwHighDateTime;
  return (time_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);
}

FileInfo** ListFiles(const char* dir, int *size) {
  *size = 0;
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;

  hFind = FindFirstFile(dir, &findFileData);
  if (hFind == INVALID_HANDLE_VALUE) {
    *size = -1;
    return NULL;
  }

  int count = 0;
  do {
    if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      count++;
  } while (FindNextFile(hFind, &findFileData) != 0);

  FileInfo **files = (FileInfo**)malloc(count * sizeof(FileInfo));

  FindClose(hFind);
  hFind = FindFirstFile(dir, &findFileData);

  do {
    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      continue;

    files[*size] = (FileInfo*)malloc(sizeof(FileInfo));
    if (files[*size] == NULL) {
      perror("Memory allocation failed");
      exit(EXIT_FAILURE);
    }
    strcpy(files[*size]->cName, findFileData.cFileName);
    files[*size]->nFileSize = (unsigned long long)findFileData.nFileSizeHigh << 32 | findFileData.nFileSizeLow;
    files[*size]->tCreationTime = FileTimeToTimeT(&findFileData.ftLastWriteTime);

    *size += 1;
  } while (FindNextFile(hFind, &findFileData) != 0);

  FindClose(hFind);

  return files;
}

void AppDataPath(char path[MAX_PATH_LENGTH]) {
  strcpy(path, getenv("APPDATA"));
  strcat(path, "\\Automata");
}
