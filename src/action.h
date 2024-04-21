#pragma once

#include <stdbool.h>
#include "types.h"

extern Action *action;
extern size_t actionCount;

void ActionSetup(void);

size_t InvalidActionsCount(void);

void FreeActions(void);

bool ActionReload(void);
