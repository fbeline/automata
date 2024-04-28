#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#include "log.h"
#include "types.h"
#include "fs.h"
#include "action.h"
#include "ui.h"
#include "keyboard.h"
#include "lua_bridge.h"

DWORD WINAPI Execute(LPVOID lpParam) {
  Action *a = (Action*)lpParam;
  LuaPcall(a->command);
  return 0;
}

void CreateDataFolder(void) {
  char path[MAX_PATH];
  AppDataPath(path);
  if (CreateDirectory(path, NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS) {
    Log(LOG_ERROR, "Failed to create directory %s", path);
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  CreateDataFolder();
  LogInit();
  LuaInitState("automata.lua");
  ActionSetup();
  KeyboardHookSetup(Execute);

  UiStart();

  FreeActions();
  KeyboardUnhook();
  LuaClose();
  LogClose();

  return 0;
}
