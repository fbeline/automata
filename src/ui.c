#include "ui.h"

#include <stdio.h>
#include <windows.h>
#include <winuser.h>

#include "config.h"
#include "fs.h"
#include "keyboard.h"
#include "log.h"
#include "resource.h"

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAYICON 1
#define IDM_EXIT 1001
#define IDM_RELOAD 1002
#define IDM_LOG 1003
#define IDM_LOG_PRESSES_KEYS 1004
#define IDM_START_WITH_WINDOWS 1005
#define IDM_SCRIPTS 3000

HWND hwndGlobal;
NOTIFYICONDATA nid;

static ScriptArray scripts;
static size_t sCount = 0;
static size_t sId = 0;

void ShowMessageBox(const char* title, const char* message) {
  MessageBox(hwndGlobal, message, title, MB_OK);
}

static void ExecuteCommand(HWND hwnd, WPARAM wParam) {
  WORD param = LOWORD(wParam); 

  if (param >= IDM_SCRIPTS) {
    sId = param - IDM_SCRIPTS;
    param = IDM_RELOAD;
  }

  switch (param) {
    case IDM_EXIT:
      PostQuitMessage(0);
      break;
    case IDM_RELOAD:
      if (sCount == 0) {
        Log(LOG_WARNING, "No avaiable lua scripts found");
      } else if (!ActionReload(scripts[sId]) || InvalidActionsCount() > 0) {
        MessageBox(hwnd, "Lua script with errors", "Error", MB_OK | MB_ICONERROR);
      }
      break;
    case IDM_LOG_PRESSES_KEYS:
      logPressedKeys = !logPressedKeys;
      break;
    case IDM_START_WITH_WINDOWS:
      ToggleStartWithWindows();
      break;
    case IDM_LOG:
      char logPath[MAX_PATH];
      if (AppDataPath(logPath) != 0)
        return;
      strcat_s(logPath, MAX_PATH, "//automata.log");

      char cmd[300];
      strcpy_s(cmd, 300, "-C Get-Content -Wait -Tail 10 ");
      strcat_s(cmd, 300, logPath);

      ShellExecute(NULL,
                   "open",
                   "powershell.exe",
                   cmd,
                   NULL,
                   SW_SHOWNORMAL);

      break;
  }
}

static void OpenTrayMenu(HWND hwnd) {
  POINT pt;
  GetCursorPos(&pt);
  HMENU hMenu = CreatePopupMenu();
  HMENU sMenu = CreatePopupMenu();

  if (ListScripts(scripts, &sCount) != 0) {
    Log(LOG_ERROR, "Failed to list scripts");
    exit(1);
  }

  for (int i = 0; i < sCount; i++) {
    UINT uFlags = i == sId ? MF_STRING | MF_CHECKED : MF_STRING;
    AppendMenu(sMenu, uFlags, IDM_SCRIPTS + i, scripts[i]); // MF_STRING | MF_CHECKED
  }

  // MENUS
  AppendMenu(hMenu, MF_POPUP, (UINT_PTR)sMenu, "Scripts");
  AppendMenu(hMenu, MF_STRING, IDM_RELOAD, "Reload");
  AppendMenu(hMenu, MF_STRING, IDM_LOG, "Log");

  UINT pressedKeysFlags = logPressedKeys ? MF_STRING | MF_CHECKED : MF_STRING;
  AppendMenu(hMenu, pressedKeysFlags, IDM_LOG_PRESSES_KEYS, "Log Pressed Keys");

  UINT swwFlags = IsStartingWithSystem() ? MF_STRING | MF_CHECKED : MF_STRING;
  AppendMenu(hMenu, swwFlags, IDM_START_WITH_WINDOWS, "Start with windows");

  AppendMenu(hMenu, MF_STRING, IDM_EXIT, "Exit");
  SetForegroundWindow(hwnd);
  TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
  DestroyMenu(hMenu);
}

static void CreateTrayIcon(HWND hwnd) {
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = ID_TRAYICON;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_TRAYICON;
  nid.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
  if (nid.hIcon == NULL) {
    Log(LOG_ERROR, "Failed to load icon.");
  }
  strcpy_s(nid.szTip, 9, "Automata");

  Shell_NotifyIcon(NIM_ADD, &nid);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE:
      CreateTrayIcon(hwnd);
      break;

    case WM_TRAYICON:
      if (lParam == WM_RBUTTONUP) OpenTrayMenu(hwnd);
      break;

    case WM_COMMAND:
      ExecuteCommand(hwnd, wParam);
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

  HWND hwndGlobal = CreateWindow("MainWindow", "Automata", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
  ShowWindow(hwndGlobal, SW_HIDE);

  Log(LOG_INFO, "Application start.");

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
