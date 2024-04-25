#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "log.h"
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
  LogInit("automata.log");
  LuaInitState();
  ActionSetup();
  KeyboardHookSetup(Execute);

  UiStart();

  FreeActions();
  KeyboardUnhook();
  LuaClose();
  LogClose();

  return 0;
}
