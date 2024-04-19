#pragma once

#include <stdbool.h>

typedef struct action {
  unsigned long pressedKey;
  char *command;
  bool valid;
} Action;
