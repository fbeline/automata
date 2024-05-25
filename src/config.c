#include "config.h"

#include <windows.h>
#include "log.h"

#define STARTUP_REGISTRY "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define APPLICATION_NAME "Automata"

LONG OpenRegistry(REGSAM samDesired, HKEY *hKey, const char* subKey) {
  LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, samDesired, hKey);
  if (result != ERROR_SUCCESS) {
    Log(LOG_ERROR, "Fail to open registry %s. Error code: %ld", subKey, result);
  }
  return result;
}

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
  HKEY hKey;
  if (OpenRegistry(KEY_READ, &hKey, subKey) != ERROR_SUCCESS)
    return 1;

  DWORD bufferSize = MAX_PATH;
  LONG result = RegQueryValueEx(hKey, entry, NULL, NULL, (LPBYTE)value, &bufferSize);
  if (result != ERROR_SUCCESS) {
    if (result != 2) // do not log when key is missing
      Log(LOG_ERROR, "Fail to read registry \"%s\". Error code: %ld", entry, result);

    RegCloseKey(hKey);
    return 2;
  }
  RegCloseKey(hKey);

  return 0;
}

int DeleteRegistryKey(const char* subKey, const char* entry) {
  HKEY hKey;
  if (OpenRegistry(KEY_SET_VALUE, &hKey, subKey) != ERROR_SUCCESS)
    return 1;

  LONG result = RegDeleteValue(hKey, entry);
  if (result != ERROR_SUCCESS) {
    Log(LOG_ERROR, "Fail to delete registry \"%s\\%s\". Error code: %ld", subKey, entry, result);
    RegCloseKey(hKey);
    return 2;
  }

  Log(LOG_INFO, "Successfully delete key \"%s\\%s\"", subKey, entry);

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
  char czExePath[MAX_PATH];
  DWORD pathSize = GetExePath(czExePath);
  if (pathSize == 0) return 1;

  HKEY hKey;
  if (OpenRegistry(KEY_SET_VALUE, &hKey, STARTUP_REGISTRY) != ERROR_SUCCESS)
    return 1;

  LONG result = RegSetValueEx(hKey, APPLICATION_NAME, 0, REG_SZ, (BYTE*)czExePath, strlen(czExePath) + 1);
  if (result == ERROR_SUCCESS) {
    Log(LOG_INFO, "Successfully added automata to windows startup: (%s)", czExePath);
  } else {
    RegCloseKey(hKey);
    Log(LOG_ERROR, "Error adding automata to windows startup. Error code: %ld", result);
    return 2;
  }

  RegCloseKey(hKey);
  return 0;
}


void ToggleStartWithWindows(void) {
  if (IsStartingWithSystem()) {
    DeleteRegistryKey(STARTUP_REGISTRY, APPLICATION_NAME);
  } else {
    StartWithWindows();
  }
}
