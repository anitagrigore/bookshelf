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

  list_insert(list, (void *)3, list->tail);

  CuAssertIntEquals(tc, 3, (int32_t)list->tail->data);

  list_insert(list, (void *)2, list->head);
  CuAssertIntEquals(tc, 2, (int32_t)list->head->next->data);

  for (int32_t i = 4; i < 100; i++)
  {
    list_insert(list, (void *)i, list->tail);
  }

  struct list_node *curr = list->head;
  for (int32_t i = 1; i < 100; i++)
  {
    if (i > 1)
    {
      /*
       * We are not at the first element, so a "prev" should exist.
       */

      CuAssertPtrNotNull(tc, curr->prev);
      CuAssertIntEquals(tc, i - 1, (int32_t)curr->prev->data);
    }

    if (i < 99)
    {
      /*
       * We are not at the last element, so a "next" should exist.
       */

      CuAssertPtrNotNull(tc, curr->next);
      CuAssertIntEquals(tc, i + 1, (int32_t)curr->next->data);
    }

    curr = curr->next;
  }

  list_free(list);
}

CuSuite *make_suite_list()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_list_create);
  SUITE_ADD_TEST(suite, test_list_insert);

  return suite;
}
