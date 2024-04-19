#include "lua_bridge.h"

#include <stdio.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "keyboard.h"

lua_State* L;

int LuaWait(lua_State* L) {
  lua_Number n = lua_tonumber(L, -1);
  Sleep(n);
  return 0;
}

int LuaPressKey(lua_State* L) {
  lua_Number keyCode = lua_tonumber(L, -1);
  PressKey(keyCode);
  return 0;
}

int LuaReleaseKey(lua_State* L) {
  lua_Number keyCode = lua_tonumber(L, -1);
  ReleaseKey(keyCode);
  return 0;
}

void LuaInitState() {
  L = luaL_newstate();

  lua_pushcfunction(L, LuaWait);
  lua_setglobal(L, "wait");

  lua_pushcfunction(L, LuaPressKey);
  lua_setglobal(L, "press_key");

  lua_pushcfunction(L, LuaReleaseKey);
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
}

Action* LuaSetupActions(size_t *size) {
  *size = lua_rawlen(L, -1);
  Action* action = (Action*)malloc(*size * sizeof(Action));

  for (int i = 0; i < *size; i++) {
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

  return action;
}

void LuaPcall(const char* fnName) {
  lua_getglobal(L, fnName);
  if (lua_isfunction(L, -1)) {
    lua_pcall(L, 0, 0, 0);
  }
}

void LuaClose() {
  lua_close(L);
}

