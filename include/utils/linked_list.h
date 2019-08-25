/**
 * \file
 * \brief Doubly linked list utility module
 */

/**
 * \defgroup linked_list Linked List
 *
 * Typical usage of a linked list using this API looks like this:
 *
 * \code{.c}
 * struct list *primes = list_create();
 * list_insert(primes, (void *)2, NULL);
 * list_insert(primes, (void *)3, NULL);
 *
 * // ...
 *
 * list_free(primes);
 * \endcode
 *
 * @{
 */

#include <stdint.h>

/**
 * \brief Node of a doubly linked list
 */
struct list_node
{
  /// The next node in the list.
  struct list_node *next;

  /// The previous node in the list.
  struct list_node *prev;

  /// The data this node holds.
  void *data;
};

/**
 * \brief Doubly-linked list
 */
struct list
{
  /// The first node of the list.
  struct list_node *head;

  /// The last node of the list.
  struct list_node *tail;
};

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
 * \param list The host list
 * \param data The data to be added to the linked list
 * \param after The node after which the data has to be inserted. If NULL, the
 * data is prepanded to the list, otherwise the data is inserted after the given
 * node. For the data to be appended to the list, `list->tail` must be given.
 *
 * \return Returns 0 if the insertion was successful or 1 if an error occurred
 */
int32_t list_insert(struct list *list, void *data, struct list_node *after);

/**
 * \brief Destroys a list and all of its nodes.
 *
 * When the nodes in a list are `free`d, their \p data pointer is not freed.
 *
 * \param list The list that has to be distroyed
 */
void list_free(struct list *list);

/** @} */
