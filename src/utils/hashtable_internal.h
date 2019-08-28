#pragma once

#include "utils/hashtable.h"

int32_t hashtable_key_cmp(struct hashtable_key a, struct hashtable_key b)
{
  if (a.size != b.size)
  {
    return 1;
  }

  uint8_t *bytes_a = (uint8_t *)a.key;
  uint8_t *bytes_b = (uint8_t *)b.key;

  int32_t i;
  for (i = 0; i < a.size; i++)
  {
    if (bytes_a[i] != bytes_b[i])
    {
      return 1;
    }
  }

  return 0;
}
