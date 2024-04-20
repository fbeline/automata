#pragma once

#include <stdbool.h>

typedef struct action {
  unsigned long pressedKey;
  bool valid;
  char *command;
} Action;
