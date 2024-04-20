#pragma once

#include "types.h"

extern Action *action;
extern size_t actionCount;

void ActionSetup(void);

void FreeActions(void);

void ActionReload(void);
