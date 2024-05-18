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

void MouseMoveTo(int x, int y) {
    INPUT input = {0};

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    // coordinates are scaled to a range of 0 to 65535 (16-bit range),
    // which represents the full screen.
    input.mi.dx = (x * 65535) / (GetSystemMetrics(SM_CXSCREEN) - 1);
    input.mi.dy = (y * 65535) / (GetSystemMetrics(SM_CYSCREEN) - 1);
    
    // Send the input event
    SendInput(1, &input, sizeof(INPUT));
}
