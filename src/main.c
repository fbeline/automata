#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "types.h"
#include "action.h"
#include "ui.h"
#include "keyboard.h"
#include "lua_bridge.h"

DWORD WINAPI Execute(LPVOID lpParam) {
  Action *a = (Action*)lpParam;
  LuaPcall(a->command);
  return 0;
}

int main(int argc, char *argv[]) {
  /* FILE* logFile = freopen("automata.log", "a", stdout); */
  /* if (logFile == NULL) { */
  /*   printf("Error opening log file."); */
  /*   return 1; */
  /* } */

  LuaInitState();
  ActionSetup();
  KeyboardHookSetup(Execute);

  fflush(stdout);

  UiStart();

  FreeActions();
  KeyboardUnhook();
  LuaClose();
  /* fclose(logFile); */

  return 0;
}
