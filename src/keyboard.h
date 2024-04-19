#pragma once

#include <Windows.h>
#include <winuser.h>
#include "types.h"

void PressKey(WORD keyCode);
void ReleaseKey(WORD keyCode);

void KeyboardHookSetup(LPTHREAD_START_ROUTINE commandRoutine);
void KeyboardSetAction(Action* action, size_t size);
void KeyboardUnhook(void);
