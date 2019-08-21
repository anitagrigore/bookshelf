#include <stdio.h>

#include "cutest/CuTest.h"

int main()
{
  CuString *output = CuStringNew();
  CuSuite *all = CuSuiteNew();

  CuSuiteRun(all);
  CuSuiteSummary(all, output);
  CuSuiteDetails(all, output);
  printf("%s\n", output->buffer);
}
