#include "keyboard.h"

#include <stdio.h>
#include <winuser.h>

#include "action.h"
#include "keycode.h"
#include "log.h"

static HHOOK keyboardHook;
static LPTHREAD_START_ROUTINE CommandRoutine;
bool logPressedKeys = false;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode < 0 || (wParam != WM_KEYDOWN && wParam != WM_SYSKEYDOWN))
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);

  KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;

  if (logPressedKeys)
    Log(LOG_INFO, "KEY PRESSED VK=%d, SCAN=%d",
        kbdStruct->vkCode,
        MapVirtualKeyEx(kbdStruct->vkCode, MAPVK_VK_TO_VSC_EX, GetKeyboardLayout(0)));

  for (size_t i = 0; i < actionCount; i++) {
    if (!action[i].valid) continue;

    bool match = true;
    WORD lastKeycodeIdx = action[i].keycodeSize - 1;

    for (size_t j = 0; j < action[i].keycodeSize; j++) {
      if (j == lastKeycodeIdx && kbdStruct->vkCode != action[i].keycode[j]) {
        match = false;
      } else if (j != lastKeycodeIdx && !(GetAsyncKeyState(action[i].keycode[j]) & 0x8000)) {
        match = false;
        break;
      }
    }

    if (match) {
      // release pressed keys (avoid trigger action multiple times)
      for (size_t j = 0; j < action[i].keycodeSize; j++) {
        ReleaseKey(action[i].keycode[j]);
      }

      HANDLE hThread = CreateThread(NULL, 0, CommandRoutine, &action[i], 0, NULL);
      if (hThread == NULL) {
        Log(LOG_ERROR, "Failed to run action.");
        break;
      }
      CloseHandle(hThread);
      return 1;
    }
  }

  return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

UINT KeyEventFlag(WORD kc) {
  for (unsigned int i = 0; i < EXTENDED_KEYS_SIZE; i++) {
    if (kc == ((int[]){EXTENDED_KEYS})[i]) return KEYEVENTF_EXTENDEDKEY;
  }

  return KEYEVENTF_SCANCODE;
}

void PressKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.wScan = MapVirtualKeyEx(keyCode, MAPVK_VK_TO_VSC, 0);
  input.ki.dwFlags = KeyEventFlag(keyCode);
  SendInput(1, &input, sizeof(INPUT));
}

void ReleaseKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.wScan = MapVirtualKeyEx(keyCode, MAPVK_VK_TO_VSC, 0);
  input.ki.dwFlags = KEYEVENTF_KEYUP | KeyEventFlag(keyCode);
  SendInput(1, &input, sizeof(INPUT));
}

void TapKey(WORD kc, DWORD t) {
  PressKey(kc);
  Sleep(t);
  ReleaseKey(kc);
}

void KeyboardWrite(const char *txt) {
  if (txt == NULL) return;
  size_t len = strlen(txt);
  for (size_t i = 0; i < len; i++) {
    // TODO: Support capital letters
    TapKey(VkKeyScan(txt[i]), 10);
  }
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
