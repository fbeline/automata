#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef struct action {
  WORD pressedKey;
  char *command;
  bool valid;
} Action;

// Global variables
Action *action;
unsigned int actionCount = 0;
HHOOK keyboardHook;
lua_State* L;

void pressKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = 0; // 0 for key press

  SendInput(1, &input, sizeof(INPUT));
}

void releaseKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(1, &input, sizeof(INPUT));
}

DWORD WINAPI ExecCommand(LPVOID lpParam) {
  Action *a = (Action*)lpParam;

  lua_getglobal(L, a->command);
  if (lua_isfunction(L, -1)) {
    lua_pcall(L, 0, 0, 0);
  }

  return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
      /* printf("Key pressed: %lu\n", kbdStruct->vkCode); */
      for (unsigned int i = 0; i < actionCount; i++) {
        if (action[i].valid && kbdStruct->vkCode == action[i].pressedKey) {
          HANDLE hThread = CreateThread(NULL, 0, ExecCommand, &action[i], 0, NULL);
          if (hThread == NULL) {
            printf("[Error]: Failed to run action.\n");
            return 1;
          } 
          CloseHandle(hThread);
        }
      }
    }
  }
  return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void Setup() {
  action = malloc(sizeof(Action) * 2);
  action->pressedKey = VK_F2;
  action->command = _strdup("ABCD");

  (action + 1)->pressedKey = VK_F4;
  (action + 1)->command = _strdup("ZXCV");

  actionCount = 2;
}

int luaWait(lua_State* L) {
  lua_Number n = lua_tonumber(L, -1);
  Sleep(n);
  return 0;
}

int luaPressKey(lua_State* L) {
  lua_Number keyCode = lua_tonumber(L, -1);
  pressKey(keyCode);
  return 0;
}

int luaReleaseKey(lua_State* L) {
  lua_Number keyCode = lua_tonumber(L, -1);
  releaseKey(keyCode);
  return 0;
}

int main(int argc, char *argv[]) {
  Setup();
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    printf("Failed to set keyboard hook\n");
    return 1;
  }

  L = luaL_newstate();
  // expose functions to lua
  lua_pushcfunction(L, luaWait);
  lua_setglobal(L, "wait");

  lua_pushcfunction(L, luaPressKey);
  lua_setglobal(L, "press_key");

  lua_pushcfunction(L, luaReleaseKey);
  lua_setglobal(L, "release_key");

  luaL_openlibs(L);
  if (luaL_dofile(L, "test.lua") != LUA_OK) {
    perror("[perror]=");
    luaL_error(L, "[LUA ERROR]= %s\n", lua_tostring(L, -1));
    exit(1);
  } 
  
  lua_getglobal(L, "actions");
  if (!lua_istable(L, -1)) {
    printf("Error: Missing 'actions' definition.");
    lua_close(L);
    exit(1);
  }
  
  int actionsSize = lua_rawlen(L, -1);
  action = (Action*)malloc(actionsSize * sizeof(Action));

  for (int i = 0; i < actionsSize; i++) {
    action[i].valid = false;
    lua_rawgeti(L, -1, i + 1);
    if (!lua_istable(L, -1)) {
      printf("Error: element %d is not a table\n", i);
      lua_pop(L, 1);
      continue;
    }

    lua_getfield(L, -1, "keycode");
    if (!lua_isnumber(L, -1)) {
      printf("Error: element %d with invalid keycode\n", i);
      lua_pop(L, 2);
      continue;
    }
    action[i].pressedKey = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "action");
    if (!lua_isstring(L, -1)) {
      printf("Error: element %d action must be a string\n", i);
      lua_pop(L, 2);
      continue;
    }
    action[i].command = _strdup(lua_tostring(L, -1));
    lua_pop(L, 2);

    action[i].valid = true;
  }

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Unhook the keyboard hook before exiting
  UnhookWindowsHookEx(keyboardHook);
  lua_close(L);
  return 0;
}
