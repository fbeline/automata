#include <stdio.h>
#include <Windows.h>
#include <winuser.h>

typedef struct action {
  WORD pressedKey;
  char *command;
} Action;

// Global variables
Action *action;
unsigned int actionCount = 0;
HHOOK keyboardHook;

void pressKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = 0; // 0 for key press

  SendInput(1, &input, sizeof(INPUT));
}

void releaseKey(WORD keyCode) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = keyCode;
  input.ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(1, &input, sizeof(INPUT));
}

DWORD WINAPI ExecCommand(LPVOID lpParam) {
  Action *a = (Action*)lpParam;

  for (int i = 0; i < strlen(a->command); i++) {
    pressKey(a->command[i]);
    Sleep(50);
    releaseKey(a->command[i]);
  }

  return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
      printf("Key pressed: %lu\n", kbdStruct->vkCode);
      for (unsigned int i = 0; i < actionCount; i++) {
        if (kbdStruct->vkCode == action[i].pressedKey) {
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

void Setup() {
  action = malloc(sizeof(Action) * 2);
  action->pressedKey = VK_F2;
  action->command = _strdup("ABCD");

  (action + 1)->pressedKey = VK_F4;
  (action + 1)->command = _strdup("ZXCV");

  actionCount = 2;
}

int main(int argc, char *argv[]) {
  Setup();

  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    printf("Failed to set keyboard hook\n");
    return 1;
  }

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Unhook the keyboard hook before exiting
  UnhookWindowsHookEx(keyboardHook);
  return 0;
}
