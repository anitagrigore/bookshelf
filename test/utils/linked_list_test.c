#include <stddef.h>

#include "cutest/CuTest.h"
#include "utils/linked_list.h"

void test_list_create(CuTest *tc)
{
  struct list *list = list_create();
  CuAssertPtrNotNull(tc, list);
  CuAssertPtrEquals(tc, NULL, list->head);
  CuAssertPtrEquals(tc, NULL, list->tail);

  list_free(list);
}

void test_list_insert(CuTest *tc)
{
  struct list *list = list_create();
  list_insert(list, (void *)1, NULL);

  CuAssertPtrNotNull(tc, list->head);
  CuAssertPtrNotNull(tc, list->tail);
  CuAssertPtrEquals(tc, list->head, list->tail);

  CuAssertIntEquals(tc, 1, (int32_t)list->head->data);

  list_insert(list, (void *)2, list->tail);

  CuAssertIntEquals(tc, 2, (int32_t)list->tail->data);

  list_insert(list, (void *)3, list->head->next);
  CuAssertIntEquals(tc, 3, (int32_t)list->head->next->next->data);

  list_free(list);
}

CuSuite *make_suite_list()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_list_create);
  SUITE_ADD_TEST(suite, test_list_insert);

  return suite;
}
