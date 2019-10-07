#include <stddef.h>
#include <stdio.h>

#include "cutest/CuTest.h"
#include "utils/clargs.h"

void test_clargs_parse(CuTest *tc)
{
  struct clargs_parser *p = clargs_create_parser();

}

struct __fixture_parse_int
{
  char raw[16];
  int32_t has_error;
  int32_t expected;
};

void test_clargs_parse_int(CuTest *tc)
{
  struct __fixture_parse_int test_cases[] = {
    {"16", 0, 16},
    {"f", 1, 0},
    {" 10 ", 0, 10},
    {"10.", 1, 0},
    {"John", 1, 0},
  };

  int32_t n = sizeof(test_cases) / sizeof(struct __fixture_parse_int);

  int32_t i;
  for (i = 0; i < n; i++)
  {
    int32_t has_error = 0;
    char error[64] = {0};
    int32_t actual = clargs_parse_int(test_cases[i].raw, NULL, &has_error, error);

    CuAssertIntEquals(tc, test_cases[i].has_error, has_error);
    if (!test_cases[i].has_error)
    {
      CuAssertIntEquals(tc, test_cases[i].expected, actual);
    }
  }
}

void test_clargs_parse_bool(CuTest *tc)
{

}

void test_clargs_parse_long(CuTest *tc)
{

}

void test_clargs_parse_string(CuTest *tc)
{

}

CuSuite *make_suite_clargs()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_clargs_parse);
  SUITE_ADD_TEST(suite, test_clargs_parse_int);
  SUITE_ADD_TEST(suite, test_clargs_parse_bool);
  SUITE_ADD_TEST(suite, test_clargs_parse_long);
  SUITE_ADD_TEST(suite, test_clargs_parse_string);

  return suite;
}
