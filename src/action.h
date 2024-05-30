#pragma once

#include <stdbool.h>

#define CMD_MAX_SIZE 16
#define KEYCODE_MAX_SIZE 3

typedef struct action {
  char command[CMD_MAX_SIZE];
  size_t keycodeSize;
  unsigned short keycode[KEYCODE_MAX_SIZE];
  bool valid;
} Action;

void ActionSetup(void);

size_t InvalidActionsCount(void);

void FreeActions(void);

bool ActionReload(const char *script);

extern Action *action;
extern size_t actionCount;
