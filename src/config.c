#include "config.h"

#include <windows.h>
#include "log.h"

#define STARTUP_REGISTRY "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define APPLICATION_NAME "Automata"

DWORD GetExePath(char *path) {
  HMODULE hModule = GetModuleHandle(NULL);

  if (hModule != NULL) {
    return GetModuleFileName(hModule, path, MAX_PATH);
  } else {
    Log(LOG_ERROR, "Error getting Automata executable path");
    return 0;
  }
}

int GetRegistryValue(const char* subKey, const char* entry, char* value) {
  DWORD bufferSize = MAX_PATH;
  HKEY hKey;
  LONG result;

  result = RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &hKey);
  if (result != ERROR_SUCCESS) {
    if (result != 2) // do not log error if registry just does not exists
      Log(LOG_ERROR, "Fail to open registry %s. Error code: %ld", subKey, result);
    return 1;
  }

  result = RegQueryValueEx(hKey, entry, NULL, NULL, (LPBYTE)value, &bufferSize);
  if (result != ERROR_SUCCESS) {
    RegCloseKey(hKey);
    Log(LOG_ERROR, "Fail to read registry \"%s\". Error code: %ld", entry, result);
    return 2;
  }
  RegCloseKey(hKey);

  return 0;
}

int DeleteRegistryKey(const char* subKey, const char* entry) {
  HKEY hKey;
  LONG result;

  result = RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_SET_VALUE, &hKey);
  if (result != ERROR_SUCCESS) {
    Log(LOG_ERROR, "Fail to open registry %s. Error code: %ld", subKey, result);
    return 1;
  }

  result = RegDeleteValue(hKey, entry);
  if (result != ERROR_SUCCESS) {
    Log(LOG_ERROR, "Fail to delete registry \"%s\". Error code: %ld", entry, result);
    RegCloseKey(hKey);
    return 2;
  }

  RegCloseKey(hKey);
  return 0;
}

bool IsStartingWithSystem(void) {
  char value[MAX_PATH];
  char exePath[MAX_PATH];
  GetExePath(exePath);
  int result = GetRegistryValue(STARTUP_REGISTRY, APPLICATION_NAME, value);

  return result == 0 && strcmp(value, exePath) == 0;
}

int StartWithWindows(void) {
  HKEY hKey;
  char czExePath[MAX_PATH];
  DWORD pathSize = GetExePath(czExePath);

  if (pathSize == 0) return 1;

  if (RegOpenKeyEx(HKEY_CURRENT_USER, STARTUP_REGISTRY, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
    if (RegSetValueEx(hKey, APPLICATION_NAME, 0, REG_SZ, (BYTE*)czExePath, strlen(czExePath) + 1) == ERROR_SUCCESS) {
      RegCloseKey(hKey);
      Log(LOG_INFO, "Successfully added automata to windows startup: (%s)", czExePath);
    } else {
      RegCloseKey(hKey);
      Log(LOG_ERROR, "Error adding automata to windows startup");
      return 2;
    }
  } else {
    Log(LOG_ERROR, "Error opening registry key");
    return 1;
  }

  return 0;
}


void ToggleStartWithWindows(void) {
  if (IsStartingWithSystem()) {
    DeleteRegistryKey(STARTUP_REGISTRY, APPLICATION_NAME);
  } else {
    StartWithWindows();
  }
}
