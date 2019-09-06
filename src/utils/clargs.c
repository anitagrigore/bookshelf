#include "utils/clargs.h"
#include "utils/hashtable.h"

#include <ctype.h>
#include <stdlib.h>

struct clargs_parser *clargs_create_parser(int32_t argc, char **argv)
{
  struct clargs_parse *p = calloc(1, sizeof(struct clargs_parse));
  p->argc = argc;
  p->argv = argv;

  p->defs = hashtable_create(32);

  return p;
}

void clargs_add_argument(
  struct clargs_parser *p,
  char *name,
  char *description,
  clargs_arg_type type,
  int32_t required,
  void *extra,
  void *value_ptr
) {
  struct clargs_arg *arg = calloc(1, sizeof(struct clargs_arg));
  arg->name = name;
  arg->description = description;
  arg->type = type;
  arg->required = required;
  arg->extra = extra;
  arg->value_ptr = value_ptr;

  return hashtable_insert(p->defs, name, arg);
}

int32_t clargs_parse_int(const char *value, void *extra, char *error)
{
  error = NULL;

  if (strlen(value) == 0)
  {
    strcpy(error, "failed to parse integer");
    return 0;
  }

  if (strlen(value) == 1 && (value[0] == '+' || value[0] == '-'))
  {
    strcpy(error, "failed to parse integer");
    return 0;
  }

  if (value[0] != '+' && value[0] != '-' && !isdigit(value[0]))
  {
    strcpy(error, "failed to parse integer");
    return 0;
  }

  int32_t i;
  for (i = 1; i < strlen(value); i++)
  {
    if (!isdigit(value[i]))
    {
      strcpy(error, "failed to parse integer");
      return 0;
    }
  }

  return atoi(value);
}

int64_t clargs_parse_long(const char *value, void *extra, char *error)
{
  error = NULL;

  if (strlen(value) == 0)
  {
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  if (strlen(value) == 1 && (value[0] == '+' || value[0] == '-'))
  {
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  if (value[0] != '+' && value[0] != '-' && !isdigit(value[0]))
  {
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  int32_t i;
  for (i = 1; i < strlen(value); i++)
  {
    if (!isdigit(value[i]))
    {
      strcpy(error, "failed to parse long integer");
      return 0;
    }
  }

  return atoll(value);
}

int32_t clargs_parse_bool(const char *value, void *extra, char *error)
{
  error = NULL;
  char *allowed = {'1', '0', 't', 'f', 'y', 'n'};

  if (strlen(value) == 0)
  {
    return 1;
  }

  if (strlen(value) == 1)
  {
    int32_t i, found = 0;
    for (i = 0; i < 6; i++)
    {
      if (value[0] == allowed[i])
      {
        found = 1;
        return 1;
      }
    }

    if (found == 0)
    {
      strcpy(error, "failed to parse bool");
      return 0;
    }

    return 1;
  }

  if (!strcmp(value, "true") && !strcmp(value, "false") && !strcmp(value, "yes") && !strcmp(value, "no"))
  {
    strcpy(error, "failed to parse bool");
    return 0;
  }

  return 1;
}

char *clargs_parse_string(const char *value, void *extra, char *error)
{
  error = NULL;

  if (strlen(value) > (int32_t)extra)
  {
    strcpy(error, "failed to parse string");
    return 0;
  }

  return value;
}
