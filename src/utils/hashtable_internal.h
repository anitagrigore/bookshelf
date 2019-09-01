#pragma once

#include "utils/hashtable.h"

#include <string.h>

int32_t hashtable_key_cmp(const char *a, const char *b)
{
  return strcmp(a, b);
}
