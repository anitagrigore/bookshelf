#pragma once

#include <stdin.h>

#include "utils/hashtable.h"

typedef enum
{
  CLARGS_TYPE_INT,
  CLARGS_TYPE_LONG,
  CLARGS_TYPE_BOOL,
  CLARGS_TYPE_STRING,
} clargs_arg_type;

struct clargs_arg
{
  char name[64];
  char description[128];
  clargs_arg_type type;
  int32_t required;
  void *extra;
  void *value_ptr;
};

struct clargs_parser
{
  int32_t argc;
  char **argv;
  struct hashtable *defs;
};

/**
 * \brief Creates a parser with the arguments given by \p argv
 *
 * \param argc The number of the arguments given; should be the \p argc as received by the main
 * function of the program
 * \param argv Array of command line arguments as received by the main function of the prigram
 * \return Returns an instance of a command line parser (that needs to be freed)
 */
struct clargs_parser *clargs_create_parser(int32_t argc, char **argv);

/**
 * \brief Adds an argument to a parser given as parameter \p p
 *
 * \param p The parser for the argument to be added
 * \param name The name of the argument
 * \param description A small description of the argument
 * \param type The type of data contained by the argument to be added
 * \param required A bool variable to know if the argument is required or not
 * \param extra An extra argument for the type provided in \p type
 * \param value_ptr A pointer to a variable where the value of the argument is to be filled
 */
void clargs_add_argument(
  struct clargs_parser *p,
  char *name,
  char *description,
  clargs_arg_type type,
  int32_t required,
  void *extra,
  void *value_ptr
);

/**
 * \brief
 *
 * \param p
 * \ cl_error An error description useful in debugging
 * \return Returns 0 if the action was successful, or 1 in case of error. If the return value is 1,
 * an error description will be given by \p cl_error argument.
 */
int32_t clargs_parse(struct clargs_parser *p, char *cl_error);

/**
 * \brief Frees the memory used by the \p p parser
 *
 * \param p The parser to be freed
 */
void clargs_free_parser(struct clargs_parser *p);

int32_t clargs_parse_int(const char *value, void *extra, int32_t *has_error, char *error);

int32_t clargs_parse_bool(const char *value, void *extra, int32_t *has_error, char *error);

int64_t clargs_parse_long(const char *value, void *extra, int32_t *has_error, char *error);

char *clargs_parse_string(const char *value, void *extra, int32_t *has_error, char *error);
