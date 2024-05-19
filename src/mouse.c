#include "mouse.h"

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

void MouseMoveTo(long x, long y) {
  INPUT input = {0};
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
  int screenLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
  int screenTop = GetSystemMetrics(SM_YVIRTUALSCREEN);

  input.mi.dx = ((x - screenLeft) * 65535) / (screenWidth - 1);
  input.mi.dy = ((y - screenTop) * 65535) / (screenHeight - 1);

  SendInput(1, &input, sizeof(INPUT));
}

POINT MousePosition(void) {
  POINT p = {-1, -1};
  GetCursorPos(&p);
  return p;
}
