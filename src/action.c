#include "action.h"

#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "lua_bridge.h"

Action *action;
size_t actionCount;

void ActionSetup(void) {
  action = LuaSetupActions(&actionCount);
}

void FreeActions(void) {
  if (actionCount == 0) return;

  free(action);
  actionCount = 0;
  action = NULL;
}

size_t InvalidActionsCount(void) {
  size_t count = 0;
  for (size_t i = 0; i < actionCount; i++) {
    if (!action[i].valid) count++;
  }

  return count;
}

bool ActionReload(const char *script) {
  Log(LOG_INFO, "Loading script=%s", script);
  FreeActions();
  LuaClose();
  bool isStateValid = LuaInitState(script);
  ActionSetup();
  return isStateValid;
}
