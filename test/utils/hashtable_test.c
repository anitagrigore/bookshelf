#include <stddef.h>
#include <stdio.h>

#include "cutest/CuTest.h"
#include "utils/hashtable.h"

void test_hashtable_create(CuTest *tc)
{
  struct hashtable *ht = hashtable_create(10);
  CuAssertIntEquals(tc, 10, ht->slots_count);

  for (int32_t i = 0; i < ht->slots_count; i++)
  {
    CuAssertPtrNotNull(tc, ht->slots[i]);
  }

  hashtable_free(ht, NULL);
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

  hashtable_free(ht, NULL);
}

int32_t hashtable_free_cb_calls = 0;
void test_hashtable_free_free_handler(void *unused)
{
  hashtable_free_cb_calls++;
}

void test_hashtable_free(CuTest *tc)
{
  struct hashtable *ht = hashtable_create(10);

  int64_t i;
  for (i = 0; i < 100; i++)
  {
    char buf[8] = {0};
    snprintf(buf, sizeof(buf), "%lld", i);

    hashtable_insert(ht, buf, (void *)i);
  }

  CuAssertIntEquals(tc, 0, hashtable_free_cb_calls);

  hashtable_free(ht, test_hashtable_free_free_handler);
  CuAssertIntEquals(tc, 100, hashtable_free_cb_calls);
}

CuSuite *make_suite_hashtable()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_hashtable_create);
  SUITE_ADD_TEST(suite, test_hashtable_insert);
  SUITE_ADD_TEST(suite, test_hashtable_free);

  return suite;
}
