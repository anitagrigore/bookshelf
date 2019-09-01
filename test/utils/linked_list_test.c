#include <stddef.h>

#include "cutest/CuTest.h"
#include "utils/linked_list.h"

void test_list_create(CuTest *tc)
{
  struct list *list = list_create();
  CuAssertPtrNotNull(tc, list);
  CuAssertPtrEquals(tc, NULL, list->head);
  CuAssertPtrEquals(tc, NULL, list->tail);

  list_free(list, NULL);
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

  list_free(list, NULL);
}

void test_list_delete(CuTest *tc)
{
  struct list *list = list_create();
  for (int32_t i = 0; i < 32; i++)
  {
    list_insert(list, (void *)i, list->tail);
  }

  list_delete(list, list->head);
  CuAssertIntEquals(tc, 1, (int32_t)list->head->data);
  CuAssertPtrEquals(tc, NULL, list->head->prev);

  list_delete(list, list->tail);
  CuAssertIntEquals(tc, 30, (int32_t)list->tail->data);
  CuAssertPtrEquals(tc, NULL, list->tail->next);

  struct list_node *curr = list->head;
  while ((int32_t)curr->data != 15)
  {
    curr = curr->next;
  }

  struct list_node *next = curr->next;
  struct list_node *prev = curr->prev;

  list_delete(list, curr);
  CuAssertIntEquals(tc, 16, (int32_t)next->data);
  CuAssertIntEquals(tc, 14, (int32_t)prev->data);
  CuAssertIntEquals(tc, 16, (int32_t)prev->next->data);
  CuAssertIntEquals(tc, 14, (int32_t)next->prev->data);

  list_free(list, NULL);
}

int32_t free_handler_calls = 0;
void test_list_free_free_handler(void *unused)
{
  free_handler_calls++;
}

void test_list_free(CuTest *tc)
{
  struct list *list = list_create();

  int32_t i;
  for (i = 0; i < 100; i++)
  {
    list_insert(list, (void *)1, NULL);
  }

  CuAssertIntEquals(tc, 0, free_handler_calls);

  /*
   * Check that, if passed, the callback is called exactly 100 times (which is the number of
   * elements in the list).
   */
  list_free(list, test_list_free_free_handler);
  CuAssertIntEquals(tc, 100, free_handler_calls);
}

CuSuite *make_suite_list()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_list_create);
  SUITE_ADD_TEST(suite, test_list_insert);
  SUITE_ADD_TEST(suite, test_list_delete);
  SUITE_ADD_TEST(suite, test_list_free);

  return suite;
}
