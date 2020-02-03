#include "utils/clargs.h"
#include "utils/hashtable.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  // POSIX strcasecmp
#include <errno.h>

struct clargs_parser *clargs_create_parser()
{
  struct clargs_parser *p = calloc(1, sizeof(struct clargs_parser));

  if ((p->defs = hashtable_create(32)) == NULL)
  {
    free(p);
    return NULL;
  }

  if ((p->_arguments = list_create()) == NULL)
  {
    hashtable_free(p->defs, NULL);
    free(p);
    return NULL;
  }

  return p;
}

int32_t clargs_add_argument(
  struct clargs_parser *p,
  char *name,
  char *description,
  clargs_arg_type type,
  int32_t required,
  void *extra,
  void *value_ptr
) {
  struct clargs_arg *arg = calloc(1, sizeof(struct clargs_arg));
  if (arg == NULL)
  {
    return CLARGS_ERR_ALLOC;
  }

  if (!name || strlen(name) == 0)
  {
    free(arg);
    return CLARGS_ERR_NAME_REQUIRED;
  }

  strncpy(arg->name, name, sizeof(arg->name));

  if (description != NULL)
  {
    strncpy(arg->description, description, sizeof(arg->description));
  }

  arg->type = type;
  arg->required = required;
  arg->extra = extra;
  arg->value_ptr = value_ptr;

  int32_t err;

  err = hashtable_insert(p->defs, name, arg);
  if (err != 0)
  {
    free(arg);
    return err;
  }

  err = LIST_APPEND(p->_arguments, (void *)arg);
  if (err != 0)
  {
    hashtable_delete(p->defs, name, NULL);
    free(arg);
    return err;
  }

  return err;
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

  errno = 0;  // properly reset errno before attempting to parse the number

  char *end;
  int32_t parsed_value = strtol(value, &end, 10);

  if (errno == ERANGE)
  {
    *has_error = 1;
    strcpy(error, "value is out of range");
    return 0;
  }

  if (end != value + strlen(value))
  {
    *has_error = 1;
    strcpy(error, "failed to parse integer");
    return 0;
  }

  return parsed_value;
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

  errno = 0;  // properly reset errno before attempting to parse the number

  char *end;
  int64_t parsed_value = strtoll(value, &end, 10);

  if (errno == ERANGE)
  {
    *has_error = 1;
    strcpy(error, "value is out of range");
    return 0;
  }

  if (end != value + strlen(value))
  {
    *has_error = 1;
    strcpy(error, "failed to parse long integer");
    return 0;
  }

  return parsed_value;
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

char *clargs_parse_string(char *value, void *extra, int32_t *has_error, char *error)
{
  *has_error = 0;

  if (extra != NULL && strlen(value) > (int32_t)extra)
  {
    *has_error = 1;
    snprintf(error, CLARGS_ERROR_SIZE, "string is longer than %d characters", (int32_t)extra);
    return NULL;
  }

  return value;
}

int32_t clargs_parse(struct clargs_parser *p, int32_t argc, const char **argv, char *cl_error)
{
  struct hashtable *found_args = hashtable_create(32);
  if (!found_args)
  {
    return CLARGS_ERR_ALLOC;
  }

  size_t i;
  for (i = 0; i < argc; i++)
  {
    if (strstr(argv[i], "--") == argv[i])
    {
      const char *arg_name = argv[i] + 2;  // 2 is the length of the prefix "--"
      int32_t arg_found = 0;
      struct clargs_arg *arg = (struct clargs_arg *)hashtable_lookup(p->defs, arg_name, &arg_found);
      if (!arg_found)
      {
        snprintf(cl_error, CLARGS_ERROR_SIZE, "unknown argument: %s", arg_name);
        return 1;
      }

      if (hashtable_insert(found_args, arg_name, NULL) != 0)
      {
        hashtable_free(found_args, NULL);
        return 1;
      }

      char error[CLARGS_ERROR_SIZE] = {0};
      int32_t has_error = 0;

      if (arg->type != CLARGS_TYPE_BOOL && (i == argc - 1 ||
        strstr(argv[i + 1], "--") == argv[i + 1]))
      {
        snprintf(cl_error, CLARGS_ERROR_SIZE, "missing values for argument %s", arg_name);
        return 1;
      }

      char v[1024] = {0};
      if (argv[i + 1])
      {
        strncpy(v, argv[i + 1], sizeof(v));
      }

      switch (arg->type)
      {
      case CLARGS_TYPE_INT:
        *((int32_t *)arg->value_ptr) = clargs_parse_int(v, arg->extra, &has_error, error);
        break;
      case CLARGS_TYPE_LONG:
        *((int64_t *)arg->value_ptr) = clargs_parse_long(v, arg->extra, &has_error, error);
        break;
      case CLARGS_TYPE_STRING:
        {
          char *value = clargs_parse_string(v, arg->extra, &has_error, error);
          if (!has_error)
          {
            size_t max_length = (size_t)arg->extra;
            if (max_length == 0)
            {
              /*
               * The user chose not to define a maximum length (which should usually correspond to
               * the allocated buffer size), so we are allowing anything here.
               */
              strcpy((char *)arg->value_ptr, value);
            }
            else
            {
              /*
               * We trat the maximum length as the size of the allocated buffer here to prevent
               * buffer overflow.
               */
              strncpy((char *)arg->value_ptr, value, max_length);
            }
          }
          break;
        }
      case CLARGS_TYPE_BOOL:
        {
          /*
           * Allocating 16 bytes for a value that's expected to be boolean is more than reasonable.
           * Anything longer than 16 bytes will get truncated and is, for sure, junk.
           */
          char value[16] = {0};
          if (i == argc - 1 || strstr(v, "--") == v)
          {
            strcpy(value, "");
          }
          else
          {
            strncpy(value, argv[i + 1], sizeof(value));
          }

          *((int32_t *)arg->value_ptr) = clargs_parse_bool(value, arg->extra, &has_error, error);
          break;
        }
      default:
        snprintf(cl_error, CLARGS_ERROR_SIZE, "unknown argument type: %d", arg->type);
        return 1;
      }

      if (has_error)
      {
        snprintf(cl_error, CLARGS_ERROR_SIZE, "%s: %s", arg_name, error);
        return 1;
      }
    }
  }

  /*
   * Check if all the required arguments have been passed.
   */

  struct list_node *curr = p->_arguments->head;
  while (curr != NULL)
  {
    struct clargs_arg *data = (struct clargs_arg *)curr->data;

    if (data->required)
    {
      int32_t found = 0;

      hashtable_lookup(found_args, data->name, &found);

      if (!found)
      {
        hashtable_free(found_args, NULL);

        snprintf(cl_error, CLARGS_ERROR_SIZE, "required argument not found: %s", data->name);
        return 1;
      }
    }

    curr = curr->next;
  }

  return 0;
}

void clargs_free_parser(struct clargs_parser *p)
{

}
