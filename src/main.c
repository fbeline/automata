#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "types.h"
#include "keyboard.h"
#include "lua_bridge.h"

DWORD WINAPI Execute(LPVOID lpParam) {
  Action *a = (Action*)lpParam;
  printf("[debug] = LUA COMMAND: %s\n", a->command);
  LuaPcall(a->command);
  return 0;
}

int main(int argc, char *argv[]) {
  size_t actionCount = 0;

  LuaInitState();
  Action *action = LuaSetupActions(&actionCount);

  KeyboardHookSetup(Execute);
  KeyboardSetAction(action, actionCount);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  LuaClose();
  KeyboardUnhook();
  return 0;
}
