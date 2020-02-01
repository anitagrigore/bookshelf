#include <stddef.h>
#include <stdio.h>

#include "cutest/CuTest.h"
#include "utils/clargs.h"

void test_clargs_parse(CuTest *tc)
{
  struct clargs_parser *p = clargs_create_parser();

  char arg_name[8] = {0};
  int32_t arg_score = 0;
  int32_t arg_flag = 0;

  clargs_add_argument(p, "name", NULL, CLARGS_TYPE_STRING, 0, (void *)8, arg_name);
  clargs_add_argument(p, "score", NULL, CLARGS_TYPE_INT, 0, NULL, &arg_score);
  clargs_add_argument(p, "flag", NULL, CLARGS_TYPE_BOOL, 1, NULL, &arg_flag);

  char error[CLARGS_ERROR_SIZE] = {0};
  char failed = 0;

  const char *valid[] = {"--name", "foo", "--score", "32", "--flag", NULL};
  int32_t valid_argc = 5;

  failed = clargs_parse(p, valid_argc, valid, error);
  CuAssertIntEquals(tc, 0, failed);
  CuAssertStrEquals(tc, "foo", arg_name);
  CuAssertIntEquals(tc, 32, arg_score);
  CuAssertIntEquals(tc, 1, arg_flag);
}

void test_clargs_add_argument(CuTest *tc)
{
  struct clargs_parser *p = clargs_create_parser();

  int32_t err = 0;
  int32_t dummy = 0;

  err = clargs_add_argument(p, "", NULL, CLARGS_TYPE_BOOL, 0, NULL, &dummy);
  CuAssertIntEquals_Msg(tc, "empty name", CLARGS_ERR_NAME_REQUIRED, err);

  err = clargs_add_argument(p, NULL, NULL, CLARGS_TYPE_BOOL, 0, NULL, &dummy);
  CuAssertIntEquals_Msg(tc, "null name", CLARGS_ERR_NAME_REQUIRED, err);

  err = clargs_add_argument(p, "foo", NULL, CLARGS_TYPE_INT, 0, NULL, &dummy);
  CuAssertIntEquals_Msg(tc, "correct addition", 0, err);
}

struct __fixture_parse_numeric_like
{
  char raw[24];
  int32_t has_error;
  int32_t expected;
};

void test_clargs_parse_int(CuTest *tc)
{
  struct __fixture_parse_numeric_like test_cases[] = {
    {"16", 0, 16},
    {"-10", 0, -10},
    {"+10", 0, 10},
    {"f", 1, 0},
    {" 10 ", 1, 10},
    {"10.", 1, 0},
    {"John", 1, 0},
    {"9999999999999999999", 1, 0},  // out of range
  };

  int32_t n = sizeof(test_cases) / sizeof(struct __fixture_parse_numeric_like);
  char trace_msg[32] = {0};

  int32_t i;
  for (i = 0; i < n; i++)
  {
    snprintf(trace_msg, sizeof(trace_msg), "test case #%d", i);

    int32_t has_error = 0;
    char error[CLARGS_ERROR_SIZE] = {0};
    int32_t actual = clargs_parse_int(test_cases[i].raw, NULL, &has_error, error);

    CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].has_error, has_error);
    if (!test_cases[i].has_error)
    {
      CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].expected, actual);
    }
  }
}

void test_clargs_parse_bool(CuTest *tc)
{
  struct __fixture_parse_numeric_like test_cases[] = {
    {"true", 0, 1},
    {"True", 0, 1},
    {"t", 0, 1},
    {"T", 0, 1},
    {"", 0, 1},
    {"yes", 0, 1},
    {"y", 0, 1},
    {"1", 0, 1},
    {"false", 0, 0},
    {"fALSE", 0, 0},
    {"False", 0, 0},
    {"f", 0, 0},
    {"no", 0, 0},
    {"n", 0, 0},
    {"0", 0, 0},
    {" false ", 1, 0},
    {" true ", 1, 0},
  };

  int32_t n = sizeof(test_cases) / sizeof(struct __fixture_parse_numeric_like);
  char trace_msg[32] = {0};

  int32_t i;
  for (i = 0; i < n; i++)
  {
    snprintf(trace_msg, sizeof(trace_msg), "test case #%d", i);

    int32_t has_error = 0;
    char error[CLARGS_ERROR_SIZE] = {0};
    int32_t actual = clargs_parse_bool(test_cases[i].raw, NULL, &has_error, error);

    CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].has_error, has_error);
    if (!test_cases[i].has_error)
    {
      CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].expected, actual);
    }
  }
}

void test_clargs_parse_long(CuTest *tc)
{
  struct __fixture_parse_numeric_like test_cases[] = {
    {"16", 0, 16},
    {"-10", 0, -10},
    {"+10", 0, 10},
    {"f", 1, 0},
    {" 10 ", 1, 10},
    {"10.", 1, 0},
    {"John", 1, 0},
    {"9999999999999999999", 1, 0},  // out of range
  };

  int32_t n = sizeof(test_cases) / sizeof(struct __fixture_parse_numeric_like);
  char trace_msg[32] = {0};

  int32_t i;
  for (i = 0; i < n; i++)
  {
    snprintf(trace_msg, sizeof(trace_msg), "test case #%d", i);

    int32_t has_error = 0;
    char error[CLARGS_ERROR_SIZE] = {0};
    int32_t actual = clargs_parse_long(test_cases[i].raw, NULL, &has_error, error);

    CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].has_error, has_error);
    if (!test_cases[i].has_error)
    {
      CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].expected, actual);
    }
  }
}

struct __fixture_parse_string
{
  char raw[24];
  size_t max_allowed_length;
  int32_t has_error;
  char expected[24];
};

void test_clargs_parse_string(CuTest *tc)
{
  struct __fixture_parse_string test_cases[] = {
    {"", 0, 0, ""},
    {"foo", 0, 0, "foo"},
    {"foo", 16, 0, "foo"},
    {"foobarbaz", 3, 1, ""},
  };

  size_t n = sizeof(test_cases) / sizeof(struct __fixture_parse_string);
  char trace_msg[32] = {0};

  int32_t i;
  for (i = 0; i < n; i++)
  {
    snprintf(trace_msg, sizeof(trace_msg), "test case #%i", i);

    int32_t has_error = 0;
    char error[CLARGS_ERROR_SIZE] = {0};
    char *actual = clargs_parse_string(test_cases[i].raw, (void *)test_cases[i].max_allowed_length,
      &has_error, error);

    CuAssertIntEquals_Msg(tc, trace_msg, test_cases[i].has_error, has_error);
    if (!test_cases[i].has_error)
    {
      CuAssertStrEquals_Msg(tc, trace_msg, test_cases[i].expected, actual);
    }
  }
}

CuSuite *make_suite_clargs()
{
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_clargs_parse);
  SUITE_ADD_TEST(suite, test_clargs_add_argument);
  SUITE_ADD_TEST(suite, test_clargs_parse_int);
  SUITE_ADD_TEST(suite, test_clargs_parse_bool);
  SUITE_ADD_TEST(suite, test_clargs_parse_long);
  SUITE_ADD_TEST(suite, test_clargs_parse_string);

  return suite;
}
