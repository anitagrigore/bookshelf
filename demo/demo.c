#include "utils/clargs.h"

#include <stdio.h>

int main(int32_t argc, const char **argv)
{
  struct clargs_parser *p = clargs_create_parser();

  if (!p)
  {
    return 1;
  }

  char name[32] = {0};
  int32_t age = 0;
  int32_t score = 0;
  int32_t share_data = 0;

  clargs_add_argument(p, "name", NULL, CLARGS_TYPE_STRING, 1, (void *)32, name);
  clargs_add_argument(p, "age", NULL, CLARGS_TYPE_INT, 1, NULL, &age);
  clargs_add_argument(p, "score", NULL, CLARGS_TYPE_INT, 0, NULL, &score);
  clargs_add_argument(p, "share_data", NULL, CLARGS_TYPE_BOOL, 0, NULL, &share_data);

  char cl_error[CLARGS_ERROR_SIZE] = {0};
  int32_t parse_result = clargs_parse(p, argc, argv, cl_error);

  if (parse_result != 0)
  {
    fprintf(stderr, "error: %s\n", cl_error);
    return 2;
  }

  printf("name: %s\nage: %d\nscore: %d\nshare_data: %s\n",
    name, age, score, share_data ? "yes" : "no");

  return 0;
}
