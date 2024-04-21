#include "action.h"

#include <stdlib.h>
#include <stdio.h>
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

void ActionReload(void) {
  printf("[info]= Automata reloading...\n");
  FreeActions();
  LuaClose();
  LuaInitState();
  ActionSetup();
  printf("[info]= Automata reloaded.\n");
}
