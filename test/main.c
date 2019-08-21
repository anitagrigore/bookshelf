#include <stdio.h>

#include "cutest/CuTest.h"

CuSuite *make_suite_list();

int main()
{
  CuString *output = CuStringNew();
  CuSuite *all = CuSuiteNew();

  CuSuiteAddSuite(all, make_suite_list());

  CuSuiteRun(all);
  CuSuiteSummary(all, output);
  CuSuiteDetails(all, output);
  printf("%s\n", output->buffer);
}
