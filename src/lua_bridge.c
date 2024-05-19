#include "lua_bridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winuser.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "log.h"
#include "keyboard.h"
#include "mouse.h"
#include "fs.h"
#include "types.h"

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
  lua_setglobal(L, "press_mouse");

  lua_pushcfunction(L, LuaMousePosition);
  lua_setglobal(L, "mouse_position");

  DECLARE_KEYCODE(L, VK_A);
  DECLARE_KEYCODE(L, VK_B);
  DECLARE_KEYCODE(L, VK_C);
  DECLARE_KEYCODE(L, VK_D);
  DECLARE_KEYCODE(L, VK_E);
  DECLARE_KEYCODE(L, VK_F);
  DECLARE_KEYCODE(L, VK_G);
  DECLARE_KEYCODE(L, VK_H);
  DECLARE_KEYCODE(L, VK_I);
  DECLARE_KEYCODE(L, VK_J);
  DECLARE_KEYCODE(L, VK_K);
  DECLARE_KEYCODE(L, VK_L);
  DECLARE_KEYCODE(L, VK_M);
  DECLARE_KEYCODE(L, VK_N);
  DECLARE_KEYCODE(L, VK_O);
  DECLARE_KEYCODE(L, VK_P);
  DECLARE_KEYCODE(L, VK_Q);
  DECLARE_KEYCODE(L, VK_R);
  DECLARE_KEYCODE(L, VK_S);
  DECLARE_KEYCODE(L, VK_T);
  DECLARE_KEYCODE(L, VK_U);
  DECLARE_KEYCODE(L, VK_V);
  DECLARE_KEYCODE(L, VK_W);
  DECLARE_KEYCODE(L, VK_X);
  DECLARE_KEYCODE(L, VK_Y);
  DECLARE_KEYCODE(L, VK_Z);

  DECLARE_KEYCODE(L, VK_0);
  DECLARE_KEYCODE(L, VK_1);
  DECLARE_KEYCODE(L, VK_2);
  DECLARE_KEYCODE(L, VK_3);
  DECLARE_KEYCODE(L, VK_4);
  DECLARE_KEYCODE(L, VK_5);
  DECLARE_KEYCODE(L, VK_6);
  DECLARE_KEYCODE(L, VK_7);
  DECLARE_KEYCODE(L, VK_8);
  DECLARE_KEYCODE(L, VK_9);

  DECLARE_KEYCODE(L,VK_LBUTTON);
  DECLARE_KEYCODE(L,VK_RBUTTON);
  DECLARE_KEYCODE(L,VK_CANCEL);
  DECLARE_KEYCODE(L,VK_MBUTTON);
  DECLARE_KEYCODE(L,VK_XBUTTON1);
  DECLARE_KEYCODE(L,VK_XBUTTON2);
  DECLARE_KEYCODE(L,VK_BACK);
  DECLARE_KEYCODE(L,VK_TAB);
  DECLARE_KEYCODE(L,VK_CLEAR);
  DECLARE_KEYCODE(L,VK_RETURN);
  DECLARE_KEYCODE(L,VK_SHIFT);
  DECLARE_KEYCODE(L,VK_CONTROL);
  DECLARE_KEYCODE(L,VK_MENU);
  DECLARE_KEYCODE(L,VK_PAUSE);
  DECLARE_KEYCODE(L,VK_CAPITAL);
  DECLARE_KEYCODE(L,VK_KANA);
  DECLARE_KEYCODE(L,VK_HANGEUL);
  DECLARE_KEYCODE(L,VK_HANGUL);
  DECLARE_KEYCODE(L,VK_IME_ON);
  DECLARE_KEYCODE(L,VK_JUNJA);
  DECLARE_KEYCODE(L,VK_FINAL);
  DECLARE_KEYCODE(L,VK_HANJA);
  DECLARE_KEYCODE(L,VK_KANJI);
  DECLARE_KEYCODE(L,VK_IME_OFF);
  DECLARE_KEYCODE(L,VK_ESCAPE);
  DECLARE_KEYCODE(L,VK_CONVERT);
  DECLARE_KEYCODE(L,VK_NONCONVERT);
  DECLARE_KEYCODE(L,VK_ACCEPT);
  DECLARE_KEYCODE(L,VK_MODECHANGE);
  DECLARE_KEYCODE(L,VK_SPACE);
  DECLARE_KEYCODE(L,VK_PRIOR);
  DECLARE_KEYCODE(L,VK_NEXT);
  DECLARE_KEYCODE(L,VK_END);
  DECLARE_KEYCODE(L,VK_HOME);
  DECLARE_KEYCODE(L,VK_LEFT);
  DECLARE_KEYCODE(L,VK_UP);
  DECLARE_KEYCODE(L,VK_RIGHT);
  DECLARE_KEYCODE(L,VK_DOWN);
  DECLARE_KEYCODE(L,VK_SELECT);
  DECLARE_KEYCODE(L,VK_PRINT);
  DECLARE_KEYCODE(L,VK_EXECUTE);
  DECLARE_KEYCODE(L,VK_SNAPSHOT);
  DECLARE_KEYCODE(L,VK_INSERT);
  DECLARE_KEYCODE(L,VK_DELETE);
  DECLARE_KEYCODE(L,VK_HELP);
  DECLARE_KEYCODE(L,VK_LWIN);
  DECLARE_KEYCODE(L,VK_RWIN);
  DECLARE_KEYCODE(L,VK_APPS);
  DECLARE_KEYCODE(L,VK_SLEEP);
  DECLARE_KEYCODE(L,VK_NUMPAD0);
  DECLARE_KEYCODE(L,VK_NUMPAD1);
  DECLARE_KEYCODE(L,VK_NUMPAD2);
  DECLARE_KEYCODE(L,VK_NUMPAD3);
  DECLARE_KEYCODE(L,VK_NUMPAD4);
  DECLARE_KEYCODE(L,VK_NUMPAD5);
  DECLARE_KEYCODE(L,VK_NUMPAD6);
  DECLARE_KEYCODE(L,VK_NUMPAD7);
  DECLARE_KEYCODE(L,VK_NUMPAD8);
  DECLARE_KEYCODE(L,VK_NUMPAD9);
  DECLARE_KEYCODE(L,VK_MULTIPLY);
  DECLARE_KEYCODE(L,VK_ADD);
  DECLARE_KEYCODE(L,VK_SEPARATOR);
  DECLARE_KEYCODE(L,VK_SUBTRACT);
  DECLARE_KEYCODE(L,VK_DECIMAL);
  DECLARE_KEYCODE(L,VK_DIVIDE);
  DECLARE_KEYCODE(L,VK_F1);
  DECLARE_KEYCODE(L,VK_F2);
  DECLARE_KEYCODE(L,VK_F3);
  DECLARE_KEYCODE(L,VK_F4);
  DECLARE_KEYCODE(L,VK_F5);
  DECLARE_KEYCODE(L,VK_F6);
  DECLARE_KEYCODE(L,VK_F7);
  DECLARE_KEYCODE(L,VK_F8);
  DECLARE_KEYCODE(L,VK_F9);
  DECLARE_KEYCODE(L,VK_F10);
  DECLARE_KEYCODE(L,VK_F11);
  DECLARE_KEYCODE(L,VK_F12);
  DECLARE_KEYCODE(L,VK_F13);
  DECLARE_KEYCODE(L,VK_F14);
  DECLARE_KEYCODE(L,VK_F15);
  DECLARE_KEYCODE(L,VK_F16);
  DECLARE_KEYCODE(L,VK_F17);
  DECLARE_KEYCODE(L,VK_F18);
  DECLARE_KEYCODE(L,VK_F19);
  DECLARE_KEYCODE(L,VK_F20);
  DECLARE_KEYCODE(L,VK_F21);
  DECLARE_KEYCODE(L,VK_F22);
  DECLARE_KEYCODE(L,VK_F23);
  DECLARE_KEYCODE(L,VK_F24);
  DECLARE_KEYCODE(L,VK_NAVIGATION_VIEW);
  DECLARE_KEYCODE(L,VK_NAVIGATION_MENU);
  DECLARE_KEYCODE(L,VK_NAVIGATION_UP);
  DECLARE_KEYCODE(L,VK_NAVIGATION_DOWN);
  DECLARE_KEYCODE(L,VK_NAVIGATION_LEFT);
  DECLARE_KEYCODE(L,VK_NAVIGATION_RIGHT);
  DECLARE_KEYCODE(L,VK_NAVIGATION_ACCEPT);
  DECLARE_KEYCODE(L,VK_NAVIGATION_CANCEL);
  DECLARE_KEYCODE(L,VK_NUMLOCK);
  DECLARE_KEYCODE(L,VK_SCROLL);
  DECLARE_KEYCODE(L,VK_OEM_NEC_EQUAL);
  DECLARE_KEYCODE(L,VK_OEM_FJ_JISHO);
  DECLARE_KEYCODE(L,VK_OEM_FJ_MASSHOU);
  DECLARE_KEYCODE(L,VK_OEM_FJ_TOUROKU);
  DECLARE_KEYCODE(L,VK_OEM_FJ_LOYA);
  DECLARE_KEYCODE(L,VK_OEM_FJ_ROYA);
  DECLARE_KEYCODE(L,VK_LSHIFT);
  DECLARE_KEYCODE(L,VK_RSHIFT);
  DECLARE_KEYCODE(L,VK_LCONTROL);
  DECLARE_KEYCODE(L,VK_RCONTROL);
  DECLARE_KEYCODE(L,VK_LMENU);
  DECLARE_KEYCODE(L,VK_RMENU);
  DECLARE_KEYCODE(L,VK_BROWSER_BACK);
  DECLARE_KEYCODE(L,VK_BROWSER_FORWARD);
  DECLARE_KEYCODE(L,VK_BROWSER_REFRESH);
  DECLARE_KEYCODE(L,VK_BROWSER_STOP);
  DECLARE_KEYCODE(L,VK_BROWSER_SEARCH);
  DECLARE_KEYCODE(L,VK_BROWSER_FAVORITES);
  DECLARE_KEYCODE(L,VK_BROWSER_HOME);
  DECLARE_KEYCODE(L,VK_VOLUME_MUTE);
  DECLARE_KEYCODE(L,VK_VOLUME_DOWN);
  DECLARE_KEYCODE(L,VK_VOLUME_UP);
  DECLARE_KEYCODE(L,VK_MEDIA_NEXT_TRACK);
  DECLARE_KEYCODE(L,VK_MEDIA_PREV_TRACK);
  DECLARE_KEYCODE(L,VK_MEDIA_STOP);
  DECLARE_KEYCODE(L,VK_MEDIA_PLAY_PAUSE);
  DECLARE_KEYCODE(L,VK_LAUNCH_MAIL);
  DECLARE_KEYCODE(L,VK_LAUNCH_MEDIA_SELECT);
  DECLARE_KEYCODE(L,VK_LAUNCH_APP1);
  DECLARE_KEYCODE(L,VK_LAUNCH_APP2);
  DECLARE_KEYCODE(L,VK_OEM_1);
  DECLARE_KEYCODE(L,VK_OEM_PLUS);
  DECLARE_KEYCODE(L,VK_OEM_COMMA);
  DECLARE_KEYCODE(L,VK_OEM_MINUS);
  DECLARE_KEYCODE(L,VK_OEM_PERIOD);
  DECLARE_KEYCODE(L,VK_OEM_2);
  DECLARE_KEYCODE(L,VK_OEM_3);
  DECLARE_KEYCODE(L,VK_GAMEPAD_A);
  DECLARE_KEYCODE(L,VK_GAMEPAD_B);
  DECLARE_KEYCODE(L,VK_GAMEPAD_X);
  DECLARE_KEYCODE(L,VK_GAMEPAD_Y);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_SHOULDER);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_SHOULDER);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_TRIGGER);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_TRIGGER);
  DECLARE_KEYCODE(L,VK_GAMEPAD_DPAD_UP);
  DECLARE_KEYCODE(L,VK_GAMEPAD_DPAD_DOWN);
  DECLARE_KEYCODE(L,VK_GAMEPAD_DPAD_LEFT);
  DECLARE_KEYCODE(L,VK_GAMEPAD_DPAD_RIGHT);
  DECLARE_KEYCODE(L,VK_GAMEPAD_MENU);
  DECLARE_KEYCODE(L,VK_GAMEPAD_VIEW);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_THUMBSTICK_UP);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_THUMBSTICK_DOWN);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT);
  DECLARE_KEYCODE(L,VK_GAMEPAD_LEFT_THUMBSTICK_LEFT);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_THUMBSTICK_UP);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT);
  DECLARE_KEYCODE(L,VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT);

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

