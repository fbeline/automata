#include "ui.h"

#include <stdio.h>
#include <windows.h>
#include "action.h"

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAYICON 1
#define IDM_EXIT 1001
#define IDM_RELOAD 1002
#define IDM_LOG 1003

HWND hwndGlobal;
NOTIFYICONDATA nid;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE:
      // Set up the NOTIFYICONDATA structure
      nid.cbSize = sizeof(NOTIFYICONDATA);
      nid.hWnd = hwnd;
      nid.uID = ID_TRAYICON;
      nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
      nid.uCallbackMessage = WM_TRAYICON;
      nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      strcpy_s(nid.szTip, 9, "Automata");

      // Add the icon to the system tray
      Shell_NotifyIcon(NIM_ADD, &nid);
      break;

    case WM_TRAYICON:
      // Handle tray icon events
      switch (lParam) {
        case WM_RBUTTONUP:
          POINT pt;
          GetCursorPos(&pt);
          HMENU hMenu = CreatePopupMenu();
          AppendMenu(hMenu, MF_STRING, IDM_RELOAD, "Reload");
          AppendMenu(hMenu, MF_STRING, IDM_LOG, "Log");
          AppendMenu(hMenu, MF_STRING, IDM_EXIT, "Exit");
          SetForegroundWindow(hwnd);
          TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
          DestroyMenu(hMenu);
          break; 
      }
      break;

    // handle command
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case IDM_EXIT:
          PostQuitMessage(0);
          break;
        case IDM_RELOAD:
          ActionReload();
          fflush(stdout);
          break;
        case IDM_LOG:
          fflush(stdout);
          ShellExecute(NULL, "open", "notepad.exe", "automata.log", NULL, SW_SHOWDEFAULT);
          break;
      }
      break;

    case WM_DESTROY:
      Shell_NotifyIcon(NIM_DELETE, &nid);
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

void UiStart(void) {
  WNDCLASS wc = { 0 };
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "MainWindow";
  RegisterClass(&wc);

  HWND hwnd = CreateWindow("MainWindow", "Automata", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
  ShowWindow(hwnd, SW_HIDE);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
