#pragma once

#include <time.h>

#define MAX_PATH 260
#define MAX_SCRIPTS 20

typedef char ScriptArray[MAX_SCRIPTS][MAX_PATH];

/**
 * Lists the scripts available in the scripts directory.
 * 
 * @param scripts An array to store the paths of the scripts.
 * @param size A pointer to a size_t variable to store the number of scripts found.
 * @return 0 on success, non-zero on failure.
 */
int ListScripts(ScriptArray scripts, size_t *size);

/**
 * Retrieves the application data path.
 * 
 * @param path A buffer to store the application data path.
 * @return 0 on success, non-zero on failure.
 */
int AppDataPath(char *path);

/**
 * Creates a default Lua script in the scripts directory.
 * 
 * @return 0 on success, non-zero on failure.
 */
int CreateDefaultLuaScript(void);
