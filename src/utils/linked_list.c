#include "utils/linked_list.h"

#include <stddef.h>

struct list *list_create()
{
  struct list *list = malloc(sizeof(struct list));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

int32_t list_insert(struct list *list, void *data, struct list_node *after)
{
  struct list_node *node = calloc(1, sizeof(struct list_node));
  if (node == NULL)
  {
    return 1;
  }

  node->data = data;

  if (list->head == NULL && list->tail == NULL)
  {
    list->head = node;
    list->tail = node;
  }
  if (after == NULL)
  {
    node->next = list->head;
    list->head->prev = node;
    list->head = node;

  }
  else if (after == list->tail)
  {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }
  else
  {
    after->next->prev = node;
    node->next = after->next;
    after->next = node;
    node->prev = after;
  }

  return 0;
}

void list_free(struct list *list)
{
  while (list->head != list->tail)
  {
    struct list_node *temp = list->head->next;
    free(list->head);
    list->head = temp;
  }
  free(list->head);
  free(list);
}
