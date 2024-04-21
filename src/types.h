#pragma once

#include <stdbool.h>

#define CMD_MAX_SIZE 16

typedef struct action {
  char command[CMD_MAX_SIZE];
  unsigned long pressedKey;
  bool valid;
} Action;
