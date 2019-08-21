#include <stdint.h>

struct list_node
{
  struct list_node *next;
  struct list_node *prev;
  void *data;
}

struct list
{
  struct list_node *head;
  struct list_node *tail;
}

/**
 * \brief Create an empty linked list
 *
 * \return Returns NULL on failure or a pointer to a heap-allocated
 * list structure
*/
struct list *list_create();

/**
 * \brief Inserts a node with the value \p data into a linked list after node
 * \p after
 *
 * \param data The data to be added to the linked list
 * \param after The node after which the data has to be inserted. If NULL, the
 * data is inserted at the end of the list.
 *
 * \return Returns 0 if the insertion was successful or 1 if an error occurred
*/
int32_t list_insert(void *data, struct list_node *after);

/**
 * \brief Destroys a list and all of its nodes
 *
 * \param list The list that has to be distroyed
*/
void list_free(struct list *list);
