#include "fs.h"

#include <corecrt_share.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "log.h"

int ListScripts(ScriptArray scripts, size_t *size) {
  char path[MAX_PATH];
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;
  *size = 0;

  if (AppDataPath(path) != 0) {
    return 1;
  }
  strcat_s(path, MAX_PATH, "\\*.lua");

  hFind = FindFirstFile(path, &findFileData);
  if (hFind == INVALID_HANDLE_VALUE)
    return 1;

  do {
    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
    if (*size >= MAX_SCRIPTS) {
      Log(LOG_WARNING, "Max of %d scripts reached", MAX_SCRIPTS);
      break;
    }

    strcpy_s(scripts[(*size)++], MAX_PATH, findFileData.cFileName);
  } while (FindNextFile(hFind, &findFileData) != 0);

  FindClose(hFind);
  return 0;
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

int CreateDefaultLuaScript(void) {
  char path[MAX_PATH];
  if (AppDataPath(path) != 0) {
    return 1;
  }
  strcat_s(path, MAX_PATH, "\\default.lua");

  FILE *filePtr;
  filePtr = _fsopen(path, "r", _SH_DENYNO);
  if (filePtr != NULL) {
    fclose(filePtr);
    return 0;
  }

  filePtr = _fsopen(path, "w", _SH_DENYNO);
  if (filePtr == NULL) {
    Log(LOG_ERROR, "Failed to create 'default.lua'");
    return 1;
  }

  fprintf(filePtr, "actions = {}");
  fclose(filePtr);
  return 0;
}
