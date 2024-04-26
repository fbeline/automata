#include "keyboard.h"

#include <stdio.h>
#include "log.h"
#include "action.h"
#include "types.h"

static HHOOK keyboardHook;
static LPTHREAD_START_ROUTINE CommandRoutine;

static Action* lastAction = NULL;
static bool shouldExecuteAction = true;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
    if (lastAction != NULL && wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
      if (kbdStruct->vkCode == lastAction->keycode[lastAction->keycodeSize - 1]) {
        shouldExecuteAction = true;
      }
    } else if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      printf("Key pressed: %lu\n", kbdStruct->vkCode);
      for (size_t i = 0; i < actionCount; i++) {
        if (!action[i].valid) continue;

        bool match = true;
        DWORD lastKeycodeIdx = action[i].keycodeSize - 1;

        for (size_t j = 0; j < action[i].keycodeSize; j++) {
          if (j == lastKeycodeIdx && kbdStruct->vkCode != action[i].keycode[j]) {
            match = false;
          } else if (j != lastKeycodeIdx && !(GetAsyncKeyState(action[i].keycode[j]) & 0x8000)) {
            match = false;
            break;
          }
        }

        if (match && shouldExecuteAction) {
          lastAction = &action[i];
          shouldExecuteAction = false;
          HANDLE hThread = CreateThread(NULL, 0, CommandRoutine, &action[i], 0, NULL);
          if (hThread == NULL) {
            Log(LOG_ERROR, "Failed to run action.");
            break;
          }
          CloseHandle(hThread);
        }
      }
    }
  }
  return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void PressKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = 0;

  SendInput(1, &input, sizeof(INPUT));
}

void ReleaseKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(1, &input, sizeof(INPUT));
}

void KeyboardHookSetup(LPTHREAD_START_ROUTINE commandRoutine) {
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    Log(LOG_ERROR, "Failed to set keyboard hook");
    exit(1);
  }

  CommandRoutine = commandRoutine;
}

void KeyboardUnhook(void) {
  UnhookWindowsHookEx(keyboardHook);
}
