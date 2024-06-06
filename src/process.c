#include "process.h"

#include <windows.h>

int Run(const char* command, const char* workingDir) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&pi, sizeof(pi));
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);

  if (!CreateProcess(
    NULL,
    (LPSTR)command,
    NULL,
    NULL,
    FALSE,
    0,
    NULL,
    workingDir,
    &si,
    &pi)) {
    return EXIT_FAILURE;
  }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return EXIT_SUCCESS;
}
