#include "mouse.h"

#include <windows.h>

void MouseLeftClick(void) {
  INPUT inputs[2] = {0};

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  inputs[1].type = INPUT_MOUSE;
  inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  SendInput(2, inputs, sizeof(INPUT));
}

void MouseRightClick(void) {
  INPUT inputs[2] = {0};

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
  inputs[1].type = INPUT_MOUSE;
  inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

  SendInput(2, inputs, sizeof(INPUT));
}
