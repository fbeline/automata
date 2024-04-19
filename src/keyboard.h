#pragma once

#include <stdbool.h>
#include <Windows.h>
#include <winuser.h>

typedef struct action {
  WORD pressedKey;
  char *command;
  bool valid;
} Action;

void PressKey(WORD keyCode);
void ReleaseKey(WORD keyCode);

void SetupKeyboardHook();
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
