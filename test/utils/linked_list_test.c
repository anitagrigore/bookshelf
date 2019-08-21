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

CuSuite *make_suite_list()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_list_create);

  return suite;
}
