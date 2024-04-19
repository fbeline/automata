#pragma once

#include <lua.h>
#include "keyboard.h"

void LuaInitState();

void LuaSetupActions(Action* action, size_t *size);

void LuaClose();
