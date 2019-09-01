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

  int32_t i;
  for (i = 0; i < slots_count; i++)
  {
    ht->slots[i] = list_create();

    if (!ht->slots[i])
    {
      return NULL;
    }
  }

  return ht;
}

int32_t hashtable_insert(struct hashtable *ht, const char *key, void *value)
{
  uint32_t slot_idx = murmur_hash((void *)key, strlen(key)) % ht->slots_count;
  struct list *slot = ht->slots[slot_idx];

  struct hashtable_entry *entry = malloc(sizeof(struct hashtable_entry));
  if (entry == NULL)
  {
    return 1;
  }

  entry->key = malloc(strlen(key) + 1);
  if (!entry->key)
  {
    return 1;
  }

  strcpy(entry->key, key);

  entry->value = value;

  return list_insert(slot, (void *)entry, slot->tail);
}

struct hashtable_entry hashtable_delete(struct hashtable *ht, const char *key, int32_t *found)
{
  uint32_t slot_idx = murmur_hash((void *)key, strlen(key)) % ht->slots_count;
  struct list *slot = ht->slots[slot_idx];

  struct hashtable_entry entry_backup = {0};

  struct list_node *node = slot->head;
  for (; node != NULL; node = node->next)
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

void *hashtable_lookup(struct hashtable *ht, const char *key, int32_t *found)
{
  if (found != NULL)
  {
    *found = 0;
  }

  uint32_t slot_idx = murmur_hash((void *)key, strlen(key)) % ht->slots_count;
  struct list *slot = ht->slots[slot_idx];

  struct list_node *node = slot->head;
  for (; node != NULL; node = node->next)
  {
    struct hashtable_entry *entry = (struct hashtable_entry *)node->data;

    if (strcmp(entry->key, key) == 0)
    {
      if (found != NULL)
      {
        *found = 1;
      }

      return entry->value;
    }
  }

  return NULL;
}
