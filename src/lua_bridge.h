#pragma once

#include <lua.h>
#include <stdbool.h>

#include "action.h"

bool LuaInitState(const char *script);

Action* LuaSetupActions(size_t *size);

void LuaClose(void);

void LuaPcall(const char* fnName);
