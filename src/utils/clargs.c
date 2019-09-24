#include "utils/clargs.h"
#include "utils/hashtable.h"

#include <ctype.h>
#include <stdlib.h>
#include <strings.h>  // POSIX strcasecmp

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

int32_t clargs_parse_int(const char *value, void *extra, int32_t *has_error, char *error)
{
  *has_error = 0;

  if (strlen(value) == 0)
  {
    *has_error = 1;
    strcpy(error, "failed to parse integer");
    return 0;
  }

  if (strlen(value) == 1 && (value[0] == '+' || value[0] == '-'))
  {
    *has_error = 1;
    strcpy(error, "failed to parse integer");
    return 0;
  }

  if (value[0] != '+' && value[0] != '-' && !isdigit(value[0]))
  {
    *has_error = 1;
    strcpy(error, "failed to parse integer");
    return 0;
  }

  int32_t i;
  for (i = 1; i < strlen(value); i++)
  {
    if (!isdigit(value[i]))
    {
      *has_error = 1;
      strcpy(error, "failed to parse integer");
      return 0;
    }
  }

  return atoi(value);
}

int64_t clargs_parse_long(const char *value, void *extra, int32_t *has_error, char *error)
{
  *has_error = 0;

  if (strlen(value) == 0)
  {
    *has_error = 1;
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  if (strlen(value) == 1 && (value[0] == '+' || value[0] == '-'))
  {
    *has_error = 1;
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  if (value[0] != '+' && value[0] != '-' && !isdigit(value[0]))
  {
    *has_error = 1;
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  int32_t i;
  for (i = 1; i < strlen(value); i++)
  {
    if (!isdigit(value[i]))
    {
      *has_error = 1;
      strcpy(error, "failed to parse long integer");
      return 0;
    }
  }

  return atoll(value);
}

int32_t clargs_parse_bool(const char *value, void *extra, int32_t *has_error, char *error)
{
  *has_error = 0;

  if (strlen(value) == 0)
  {
    /*
     * Flags can be passed as --use_logs without any value so we take a TRUE out of this.
     */
    return 1;
  }

  const char *allowed_true[] = {"1", "t", "y", "true", "yes", NULL};
  const char *allowed_false[] = {"0", "f", "n", "false", "no", NULL};

  int32_t i = 0;
  while (allowed_true[i] != NULL)
  {
    if (strcasecmp(value, allowed_true[i]) == 0)
    {
      return 1;
    }

    i++;
  }

  i = 0;
  while (allowed_false[i] != NULL)
  {
    if (strcasecmp(value, allowed_false[i]) == 0)
    {
      return 0;
    }

    i++;
  }

  *has_error = 1;
  strcpy(error, "failed to parse bool");
  return 0;
}

char *clargs_parse_string(const char *value, void *extra, int32_t *has_error, char *error)
{
  *has_error = 0;

  if (strlen(value) > (int32_t)extra)
  {
    *has_error = 1;
    strcpy(error, "failed to parse string");
    return NULL;
  }

  return value;
}

int32_t clargs_parse(struct clargs_parser *p, char *cl_error)
{
  int i;
  for (i = 0; i < p->argc; i++)
  {
    if (strstr(p->argv[i], "--") == p->argv[i])
    {
      char *arg_name = argv[i] + 2;  // 2 is the length of the prefix "--"
      struct clargs_arg *arg = (struct clargs_arg *)hashtable_lookup(p->defs, arg_name);
      if (arg == NULL)
      {
        snprintf(cl_error, 100, "unknown argument: %s", arg_name]);
        return 1;
      }
      else
      {
        char error[64] = {0};
        int32_t has_error = 0;

        if (arg->type != CLARGS_TYPE_BOOL && (i == argc - 1 ||
          strstr(p->argv[i + 1], "--") == p->argv[i + 1]))
        {
          snprintf(cl_error, 100, "missing values for srgument %s", arg_name);
          return 1;
        }

        switch (arg->type)
        {
        case CLARGS_TYPE_INT:
          *arg->value_ptr = clargs_parse_int(p->argv[i + 1], arg->extra, &has_error, error);
          break;
        case CLARGS_TYPE_LONG:
          *arg->value_ptr = clargs_parse_long(p->argv[i + 1], arg->extra, &has_error, error);
          break;
        case CLARGS_TYPE_STRING:
          {
            char *value = clargs_parse_string(p->argv[i + 1], arg->extra, &has_error, error);
            if (!*has_error)
            {
              strcpy(arg->value_ptr, value);
            }
            break;
          }
        case CLARGS_TYPE_BOOL:
          {
            char value[66] = {0};
            if (!p->argv[i + 1] || strstr(p->argv[i + 1], "--") == p->argv[i + 1])
            {
              strcpy(value, "");
            }
            else {
              snprintf(value, sizeof(value), "%s", p->argv[i + 1]);
            }

            *arg->value_ptr = clargs_parse_long(value, arg->extra, &has_error, error);
            break;
          }
        default:
          snprintf(cl_error, 100, "unknown argument type: %d", arg->type);
          return 1;
        }

        if (has_error)
        {
          snprintf(cl_error, 100, "%s: %s", arg_name, error);
          return 1;
        }
      }
    }
  }
}
