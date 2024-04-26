#pragma once

#include <stdbool.h>
#include <lua.h>
#include "types.h"

bool LuaInitState(const char *script);

Action* LuaSetupActions(size_t *size);

void LuaClose(void);

void LuaPcall(const char* fnName);
