#pragma once

#include <time.h>

#define MAX_PATH 260
#define MAX_SCRIPTS 20

typedef char ScriptArray[MAX_SCRIPTS][MAX_PATH];

int ListScripts(ScriptArray scripts, size_t *size);

int AppDataPath(char *path);

int CreateDefaultLuaScript(void);
