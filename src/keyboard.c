#include "keyboard.h"

static HHOOK keyboardHook;

DWORD WINAPI ExecCommand(LPVOID lpParam) {
  Action *a = (Action*)lpParam;

  lua_getglobal(L, a->command);
  if (lua_isfunction(L, -1)) {
    lua_pcall(L, 0, 0, 0);
  }

  return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
      /* printf("Key pressed: %lu\n", kbdStruct->vkCode); */
      for (unsigned int i = 0; i < actionCount; i++) {
        if (action[i].valid && kbdStruct->vkCode == action[i].pressedKey) {
          HANDLE hThread = CreateThread(NULL, 0, ExecCommand, &action[i], 0, NULL);
          if (hThread == NULL) {
            printf("[Error]: Failed to run action.\n");
            return 1;
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
  input.ki.dwFlags = 0; // 0 for key press

  SendInput(1, &input, sizeof(INPUT));
}

void ReleaseKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(1, &input, sizeof(INPUT));
}

void SetupKeyboardHook() {
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    printf("Failed to set keyboard hook\n");
    exit(1);
  }
}
