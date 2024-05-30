#pragma once

#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>

#include "action.h"

void PressKey(WORD keyCode);

void ReleaseKey(WORD keyCode);

void TapKey(WORD kc, DWORD t);

void KeyboardWrite(const char *txt);

void KeyboardHookSetup(LPTHREAD_START_ROUTINE commandRoutine);

void KeyboardSetAction(Action* action, size_t size);

void KeyboardUnhook(void);

extern bool logPressedKeys;
