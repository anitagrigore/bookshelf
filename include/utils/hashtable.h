#pragma once

#include "hash.h"
#include "linked_list.h"

#include <stddef.h>
#include <string.h>

struct hashtable
{
  size_t slots_count;
  struct list **slots;
};

struct hashtable_entry
{
  void *value;
  char *key;
};

/**
 * \brief Create a hashtable with \p slots_count slots
 *
 * \param slots_count The number of slots
 * \return Returns a pointer to a dynamically allocated hashtable
 */
struct hashtable *hashtable_create(size_t entries_count);

/**
 * \brief Insert a new element in the \p ht hashtable
 *
 * \param ht A pointer to the hashtable in which the insertion is made
 * \param key The key of the new element
 * \param value The value of the new element
 *
 * \return Returns 1 in case of error, and 0 if the inserion was successful
 */
int32_t hashtable_insert(struct hashtable *ht, const char *key, void *value);

/**
 * \brief Deletes the element with the \p key given
 *
 * \param ht The hashtable that contains the entry to be deleted
 * \param key The key of the element that has to be deleted
 * \param found Whether the entry has actually been found or not. This will be set to 1 if an
 * element with key \p key is found, 0 otherwise
 * \return Returns the value of the deleted entry for further cleanup
 */
void *hashtable_delete(struct hashtable *ht, const char *key, int32_t *found);

/**
 * \brief Look for an element with the given key
 *
 * \param ht The hash table to be searched in
 * \param key The key of the element that is searched
 * \param found A pointer to a flag marking whether the \p key has actually been found in the hash
 * table or not. If \p NULL is passed, this value will simply be ignored.
 * \return Returns the value of the element
 */
void *hashtable_lookup(struct hashtable *ht, const char *key, int32_t *found);

/**
 * \brief Destroys the hashtable and all of its entries
 *
 * \param ht A pointer to the hashtable that has to be distroyed
 */
void hashtable_free(struct hashtable *ht);
