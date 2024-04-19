#pragma once

#include <lua.h>
#include "types.h"

void LuaInitState();

Action* LuaSetupActions(size_t *size);

void LuaClose();

void LuaPcall(const char* fnName);
