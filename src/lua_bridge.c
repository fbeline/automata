#include "lua_bridge.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winuser.h>

#include "fs.h"
#include "log.h"
#include "keyboard.h"
#include "keycode.h"
#include "mouse.h"

#define DECLARE_KEYCODE(L, keycode) \
lua_pushnumber(L, keycode); \
lua_setglobal(L, #keycode);

lua_State* L;

static int LuaWait(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "wait param must be a number");
    lua_pop(L, 1);
    return 1;
  }

  lua_Number n = lua_tonumber(L, -1);
  Sleep(n);
  return 0;
}

static int LuaPressKey(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "press_key param must be a number");
    lua_pop(L, 1);
    return 1;
  }
  lua_Number keyCode = lua_tonumber(L, -1);
  PressKey(keyCode);
  return 0;
}

static int LuaReleaseKey(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "release_key param must be a number");
    lua_pop(L, 1);
    return 1;
  }
  lua_Number keyCode = lua_tonumber(L, -1);
  ReleaseKey(keyCode);
  return 0;
}

static int LuaTapKey(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "tap_key param must be a valid virtual key");
    lua_pop(L, 1);
    return 1;
  }
  lua_Number kc = lua_tonumber(L, -1);
  TapKey(kc, 50);

  return 0;
}

static int LuaWrite(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isstring(L, -1)) {
    Log(LOG_ERROR, "Write param must be a string");
    lua_pop(L, 1);
    return 1;
  }
  const char* str = lua_tostring(L, -1);
  KeyboardWrite(str);

  return 0;
}

static int LuaMouseMoveTo(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "mouse_move params must be a number");
    lua_pop(L, 1);
    return 1;
  }
  lua_Number y = lua_tonumber(L, -1);
  lua_pop(L, 1);

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "mouse_move params must be a number");
    lua_pop(L, 2);
    return 1;
  }
  lua_Number x = lua_tonumber(L, -1);
  lua_pop(L, 1);

  MouseMoveTo(x, y);
  return 0;
}

static int LuaMousePressButton(lua_State* L) {
  if (L == NULL) return 1;

  if (!lua_isnumber(L, -1)) {
    Log(LOG_ERROR, "press_mouse with invalid param");
    lua_pop(L, 1);
    return 1;
  }
  lua_Number kc = lua_tonumber(L, -1);

  switch ((int)kc)  {
    case M_LEFT:
      MouseLeftClick();
      break;
    case M_RIGHT:
      MouseRightClick();
      break;
    default:
      Log(LOG_WARNING, "Invalid mouse button");
  }

  return 0;
}

static int LuaMousePosition(lua_State *L) {
  if (L == NULL) return 1;

  POINT p = MousePosition();

  lua_newtable(L);

  lua_pushnumber(L, p.x);
  lua_setfield(L, -2, "x");
  lua_pushnumber(L, p.y);
  lua_setfield(L, -2, "y");

  return 1;
}

static int Run(lua_State *L) {
  if (L == NULL) return 1;

  if (!lua_isstring(L, -1)) {
    Log(LOG_ERROR, "Run param must be a string");
    lua_pop(L, 1);
    return 1;
  }
  const char* command = lua_tostring(L, -1);

  int result = system(command);
  Log(LOG_INFO, "Command \"%s\" executed with return status: %d", command, result);
  lua_pushinteger(L, result);

  return 0;
}

static void DeclareGlobals(void) {
  if (L == NULL) return;

  lua_pushcfunction(L, LuaWait);
  lua_setglobal(L, "wait");

  lua_pushcfunction(L, LuaPressKey);
  lua_setglobal(L, "press_key");

  lua_pushcfunction(L, LuaReleaseKey);
  lua_setglobal(L, "release_key");

  lua_pushcfunction(L, LuaTapKey);
  lua_setglobal(L, "tap_key");

  lua_pushcfunction(L, LuaWrite);
  lua_setglobal(L, "write");

  lua_pushcfunction(L, LuaMouseMoveTo);
  lua_setglobal(L, "mouse_move");

  lua_pushcfunction(L, LuaMousePressButton);
  lua_setglobal(L, "mouse_click");

  lua_pushcfunction(L, LuaMousePosition);
  lua_setglobal(L, "mouse_position");

  lua_pushcfunction(L, Run);
  lua_setglobal(L, "run");

  DECLARE_KEYCODE(L, KC_A);
  DECLARE_KEYCODE(L, KC_B);
  DECLARE_KEYCODE(L, KC_C);
  DECLARE_KEYCODE(L, KC_D);
  DECLARE_KEYCODE(L, KC_E);
  DECLARE_KEYCODE(L, KC_F);
  DECLARE_KEYCODE(L, KC_G);
  DECLARE_KEYCODE(L, KC_H);
  DECLARE_KEYCODE(L, KC_I);
  DECLARE_KEYCODE(L, KC_J);
  DECLARE_KEYCODE(L, KC_K);
  DECLARE_KEYCODE(L, KC_L);
  DECLARE_KEYCODE(L, KC_M);
  DECLARE_KEYCODE(L, KC_N);
  DECLARE_KEYCODE(L, KC_O);
  DECLARE_KEYCODE(L, KC_P);
  DECLARE_KEYCODE(L, KC_Q);
  DECLARE_KEYCODE(L, KC_R);
  DECLARE_KEYCODE(L, KC_S);
  DECLARE_KEYCODE(L, KC_T);
  DECLARE_KEYCODE(L, KC_U);
  DECLARE_KEYCODE(L, KC_V);
  DECLARE_KEYCODE(L, KC_W);
  DECLARE_KEYCODE(L, KC_X);
  DECLARE_KEYCODE(L, KC_Y);
  DECLARE_KEYCODE(L, KC_Z);
  DECLARE_KEYCODE(L, KC_0);
  DECLARE_KEYCODE(L, KC_1);
  DECLARE_KEYCODE(L, KC_2);
  DECLARE_KEYCODE(L, KC_3);
  DECLARE_KEYCODE(L, KC_4);
  DECLARE_KEYCODE(L, KC_5);
  DECLARE_KEYCODE(L, KC_6);
  DECLARE_KEYCODE(L, KC_7);
  DECLARE_KEYCODE(L, KC_8);
  DECLARE_KEYCODE(L, KC_9);

  DECLARE_KEYCODE(L, KC_NUMLOCK);
  DECLARE_KEYCODE(L, KC_NUMPAD0);
  DECLARE_KEYCODE(L, KC_NUMPAD1);
  DECLARE_KEYCODE(L, KC_NUMPAD2);
  DECLARE_KEYCODE(L, KC_NUMPAD3);
  DECLARE_KEYCODE(L, KC_NUMPAD4);
  DECLARE_KEYCODE(L, KC_NUMPAD5);
  DECLARE_KEYCODE(L, KC_NUMPAD6);
  DECLARE_KEYCODE(L, KC_NUMPAD7);
  DECLARE_KEYCODE(L, KC_NUMPAD8);
  DECLARE_KEYCODE(L, KC_NUMPAD9);
  DECLARE_KEYCODE(L, KC_MULTIPLY);
  DECLARE_KEYCODE(L, KC_ADD);
  DECLARE_KEYCODE(L, KC_SEPARATOR);
  DECLARE_KEYCODE(L, KC_SUBTRACT);
  DECLARE_KEYCODE(L, KC_DECIMAL);
  DECLARE_KEYCODE(L, KC_DIVIDE);
  DECLARE_KEYCODE(L, KC_F1);
  DECLARE_KEYCODE(L, KC_F2);
  DECLARE_KEYCODE(L, KC_F3);
  DECLARE_KEYCODE(L, KC_F4);
  DECLARE_KEYCODE(L, KC_F5);
  DECLARE_KEYCODE(L, KC_F6);
  DECLARE_KEYCODE(L, KC_F7);
  DECLARE_KEYCODE(L, KC_F8);
  DECLARE_KEYCODE(L, KC_F9);
  DECLARE_KEYCODE(L, KC_F10);
  DECLARE_KEYCODE(L, KC_F11);
  DECLARE_KEYCODE(L, KC_F12);
  DECLARE_KEYCODE(L, KC_F13);
  DECLARE_KEYCODE(L, KC_F14);
  DECLARE_KEYCODE(L, KC_F15);
  DECLARE_KEYCODE(L, KC_F16);
  DECLARE_KEYCODE(L, KC_F17);
  DECLARE_KEYCODE(L, KC_F18);
  DECLARE_KEYCODE(L, KC_F19);
  DECLARE_KEYCODE(L, KC_F20);
  DECLARE_KEYCODE(L, KC_F21);
  DECLARE_KEYCODE(L, KC_F22);
  DECLARE_KEYCODE(L, KC_F23);
  DECLARE_KEYCODE(L, KC_F24);
  DECLARE_KEYCODE(L, KC_LSHIFT);
  DECLARE_KEYCODE(L, KC_RSHIFT);
  DECLARE_KEYCODE(L, KC_LCONTROL);
  DECLARE_KEYCODE(L, KC_RCONTROL);
  DECLARE_KEYCODE(L, KC_LMENU);
  DECLARE_KEYCODE(L, KC_RMENU);
  DECLARE_KEYCODE(L, KC_LWIN);
  DECLARE_KEYCODE(L, KC_RWIN);
  DECLARE_KEYCODE(L, KC_ESCAPE);
  DECLARE_KEYCODE(L, KC_SPACE);
  DECLARE_KEYCODE(L, KC_PRIOR);
  DECLARE_KEYCODE(L, KC_NEXT);
  DECLARE_KEYCODE(L, KC_END);
  DECLARE_KEYCODE(L, KC_HOME);
  DECLARE_KEYCODE(L, KC_LEFT);
  DECLARE_KEYCODE(L, KC_UP);
  DECLARE_KEYCODE(L, KC_RIGHT);
  DECLARE_KEYCODE(L, KC_DOWN);
  DECLARE_KEYCODE(L, KC_SELECT);
  DECLARE_KEYCODE(L, KC_PRINT);
  DECLARE_KEYCODE(L, KC_EXECUTE);
  DECLARE_KEYCODE(L, KC_SNAPSHOT);
  DECLARE_KEYCODE(L, KC_INSERT);
  DECLARE_KEYCODE(L, KC_DELETE);
  DECLARE_KEYCODE(L, KC_HELP);
  DECLARE_KEYCODE(L, KC_SHIFT);
  DECLARE_KEYCODE(L, KC_CONTROL);
  DECLARE_KEYCODE(L, KC_MENU);
  DECLARE_KEYCODE(L, KC_PAUSE);
  DECLARE_KEYCODE(L, KC_CAPITAL);
  DECLARE_KEYCODE(L, KC_BACK);
  DECLARE_KEYCODE(L, KC_TAB);
  DECLARE_KEYCODE(L, KC_CLEAR);
  DECLARE_KEYCODE(L, KC_RETURN);

  // mouse buttons
  DECLARE_KEYCODE(L, M_LEFT);
  DECLARE_KEYCODE(L, M_RIGHT);
}

bool LuaInitState(const char *script) {
  char path[MAX_PATH];
  AppDataPath(path);
  strcat_s(path, MAX_PATH, "\\");
  strcat_s(path, MAX_PATH, script);

  L = luaL_newstate();
  DeclareGlobals();
  luaL_openlibs(L);
  if (luaL_dofile(L, path) != LUA_OK) {
    Log(LOG_ERROR, lua_tostring(L, -1));
    lua_close(L);
    L = NULL;
    return false;
  } 

  lua_getglobal(L, "actions");
  if (!lua_istable(L, -1)) {
    Log(LOG_WARNING, "Missing 'actions' definition.");
    lua_close(L);
    L = NULL;
    return false;
  }

  return true;
}

Action* LuaSetupActions(size_t *size) {
  if (L == NULL) {
    Log(LOG_WARNING, "Lua State is NULL");
    *size = 0;
    return NULL;
  }

  *size = lua_rawlen(L, -1);
  Action* action = (Action*)malloc(*size * sizeof(Action));
  if (action == NULL) return NULL;

  for (int i = 0; i < *size; i++) {
    action[i].valid = false;
    lua_rawgeti(L, -1, i + 1);
    if (!lua_istable(L, -1)) {
      Log(LOG_ERROR, "Action %d is not a table", i);
      lua_pop(L, 1);
      continue;
    }

    lua_getfield(L, -1, "keycode");
    if (!lua_istable(L, -1)) {
      Log(LOG_ERROR, "Action %d with invalid keycode sequence", i);
      lua_pop(L, 2);
      continue;
    }

    int keycodeSize = lua_rawlen(L, -1);
    action[i].keycodeSize = min(keycodeSize, KEYCODE_MAX_SIZE);

    int invalidKeycode = -1;
    for (int j = 0; j < KEYCODE_MAX_SIZE; j++) {
      if (j >= keycodeSize) {
        action[i].keycode[j] = 0;
        continue;
      }

      lua_pushnumber(L, j + 1);
      lua_gettable(L, -2);

      if (!lua_isnumber(L, -1)) {
        Log(LOG_ERROR,"Action %d with invalid keycode at index=%d, ", j, i);
        lua_pop(L, 1);
        invalidKeycode = j;
        break;
      }

      action[i].keycode[j] = lua_tonumber(L, -1);
      lua_pop(L, 1);
    }
    lua_pop(L, 1);

    if (invalidKeycode != -1) {
      Log(LOG_ERROR, "Action %d with invalid keycode sequence at index=%d", i, invalidKeycode);
      lua_pop(L, 1);
      continue;
    }

    lua_getfield(L, -1, "action");
    if (!lua_isstring(L, -1)) {
      Log(LOG_ERROR, "Action %d must be a string", i);
      lua_pop(L, 2);
      continue;
    }

    const char* command = lua_tostring(L, -1);
    if (strlen(command) >= CMD_MAX_SIZE) {
      Log(LOG_ERROR, "Action %d name is longer than %d.\n", i, CMD_MAX_SIZE - 1);
      lua_pop(L, 2);
      continue;
    }

    strncpy_s(action[i].command, CMD_MAX_SIZE, command, _TRUNCATE);
    lua_pop(L, 2);

    action[i].valid = true;
  }

  return action;
}

void LuaPcall(const char* fnName) {
  if (L == NULL) {
    Log(LOG_WARNING, "[LuaPcall] Lua State is NULL.");
    return;
  }

  lua_getglobal(L, fnName);
  if (lua_isfunction(L, -1)) {
    Log(LOG_INFO, "Executing %s...", fnName);
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
      Log(LOG_ERROR, lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  } else {
    Log(LOG_WARNING, "Command not found: %s", fnName);
  }
}

void LuaClose(void) {
  if (L == NULL) return;

  lua_close(L);
  L = NULL;
}

