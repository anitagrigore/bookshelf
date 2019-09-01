#include <stddef.h>
#include <stdio.h>

#include "cutest/CuTest.h"
#include "utils/hashtable.h"

void test_hashtable_create(CuTest *tc)
{
  struct hashtable *hashtable = hashtable_create(10);
  CuAssertIntEquals(tc, 10, hashtable->slots_count);

  for (int32_t i = 0; i < hashtable->slots_count; i++)
  {
    CuAssertPtrNotNull(tc, hashtable->slots[i]);
  }
}

void test_hashtable_insert(CuTest *tc)
{
  struct hashtable *ht = hashtable_create(10);
  int32_t i;
  for (i = 0; i < 100; i++)
  {
    char key_buf[8] = {0};
    snprintf(key_buf, sizeof(key_buf), "%d", i);

    hashtable_insert(ht, key_buf, (void *)i);
  }

  for (i = 0; i < 100; i++)
  {
    char key_buf[8] = {0};
    snprintf(key_buf, sizeof(key_buf), "%d", i);

    int32_t found;
    void *val = hashtable_lookup(ht, key_buf, &found);

    CuAssertTrue(tc, found);
    CuAssertIntEquals(tc, i, (int32_t)val);
  }
}

CuSuite *make_suite_hashtable()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_hashtable_create);
  SUITE_ADD_TEST(suite, test_hashtable_insert);

  return suite;
}
