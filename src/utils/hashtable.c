#include "utils/hashtable.h"
#include "utils/hash.h"
#include "hashtable_internal.h"

#include <stdlib.h>

struct hashtable *hashtable_create(size_t slots_count)
{
  struct hashtable *ht = malloc(sizeof(struct hashtable));
  if (ht == NULL)
  {
    return NULL;
  }

  ht->slots_count = slots_count;
  ht->slots = calloc(slots_count, sizeof(struct list *));
  if (ht->slots == NULL)
  {
    return NULL;
  }

  for (int32_t i = 0; i < slots_count; i++)
  {
    ht->slots[i] = list_create();
  }

  return ht;
}

int32_t hashtable_insert(struct hashtable *ht, struct hashtable_key key, void *value)
{
  uint32_t index = murmur_hash(key.key, key.size) % ht->slots_count;
  struct hashtable_entry *entry = malloc(sizeof(struct hashtable_entry));
  if (entry == NULL)
  {
    return 1;
  }

  entry->key = key;
  entry->value = value;

  if (list_insert(ht->slots[index], entry, ht->slots[index]->tail) != 0)
  {
    return 1;
  }

  return 0;
}

struct hashtable_entry hashtable_delete(struct hashtable *ht, ht_key_t key, int32_t *found)
{
  uint32_t slot_idx = murmur_hash(key.key, key.size) % ht->slots_count;

  struct hashtable_entry entry_backup = {0};

  struct list_node *node;
  for (node = ht->slots[slot_idx]->head; node != NULL; node = node->next)
  {
    struct hashtable_entry *entry = (struct hashtable_entry *)node->data;
    if (hashtable_key_cmp(entry->key, key) == 0)
    {
      *found = 1;

      entry_backup = *entry;
      list_delete(ht->slots[slot_idx], node);
      free(entry);

      return entry_backup;
    }
  }

  *found = 0;

  return entry_backup;
}

void *hashtable_lookup(struct hashtable *ht, struct hashtable_key key)
{
  uint32_t slot_idx = murmur_hash(key.key, key.size) % ht->slots_count;

  struct list_node *node;
  for (node = ht->slots[slot_idx]->head; node != NULL; node = node->next)
  {
    struct hashtable_entry *entry = (struct hashtable_entry *)node->data;
    if (hashtable_key_cmp(entry->key, key) == 0)
    {
      return entry->value;
    }
  }

  return NULL;
}
