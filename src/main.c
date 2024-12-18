#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#include "action.h"
#include "fs.h"
#include "keyboard.h"
#include "log.h"
#include "lua_bridge.h"
#include "ui.h"

DWORD WINAPI Execute(LPVOID lpParam) {
  Action *a = (Action*)lpParam;
  LuaPcall(a->command);
  return 0;
}

void CreateDataFolder(void) {
  char path[MAX_PATH];
  
  if (AppDataPath(path) != 0 ||
    (!CreateDirectory(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)) {
    printf("Failed to create directory %s\n", path);
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  CreateDataFolder();

  LogInit();

  if (CreateDefaultLuaScript() != 0) {
    Log(LOG_ERROR, "Failed to find/create 'default.lua'");
    LogClose();
    return 1;
  }

  if (!LuaInitState("default.lua")) {
    Log(LOG_ERROR, "Failed to initialize Lua state");
    LogClose();
    return 1;
  }

  ActionSetup();
  KeyboardHookSetup(Execute);

  UiStart();

  FreeActions();
  KeyboardUnhook();
  LuaClose();
  LogClose();

  return 0;
}
