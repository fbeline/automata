#pragma once

#include <lua.h>
#include "types.h"
#include <stdbool.h>

bool LuaInitState(void);

Action* LuaSetupActions(size_t *size);

void LuaClose(void);

void LuaPcall(const char* fnName);
