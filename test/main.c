#include <stdio.h>
#include <stdlib.h>

#include "cutest/CuTest.h"

CuSuite *make_suite_list();
CuSuite *make_suite_hashtable();

int main()
{
  CuString *output = CuStringNew();
  CuSuite *all = CuSuiteNew();

  CuSuite *suites[] = {
    make_suite_list(),
    make_suite_hashtable(),
  };

  const int32_t suites_count = sizeof(suites) / sizeof(CuSuite *);

  int32_t suite_idx;
  for (suite_idx = 0; suite_idx < suites_count; suite_idx++)
  {
    CuSuiteAddSuite(all, suites[suite_idx]);
  }

  CuSuiteRun(all);
  CuSuiteSummary(all, output);
  CuSuiteDetails(all, output);
  printf("%s\n", output->buffer);

  for (suite_idx = 0; suite_idx < suites_count; suite_idx++)
  {
    CuSuiteDelete(suites[suite_idx]);
  }

  CuStringDelete(output);
  free(all);
}
