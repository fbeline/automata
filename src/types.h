#pragma once

#include <stdbool.h>

#define CMD_MAX_SIZE 16
#define KEYCODE_MAX_SIZE 3

typedef struct action {
  char command[CMD_MAX_SIZE];
  unsigned long keycode[KEYCODE_MAX_SIZE];
  unsigned short keycodeSize;
  bool valid;
} Action;
