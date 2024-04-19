#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "keyboard.h"
#include "lua_bridge.h"

// Global variables
Action *action;
size_t actionCount = 0;
HHOOK keyboardHook;

int main(int argc, char *argv[]) {
  LuaInitState();
  LuaSetupActions(action, &actionCount);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Unhook the keyboard hook before exiting
  LuaClose();
  UnhookWindowsHookEx(keyboardHook);
  return 0;
}
