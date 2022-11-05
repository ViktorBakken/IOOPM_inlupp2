#include "hash_table.h"

// bucket destroy nu med svansrekursion!
static void bucket_destroy(entry_t *entry) // recursively destroys a bucket (input is pointer to first entry(dvs dummy entry)){
{
  entry_t *next = entry->next;
  free(entry);
  if (next != NULL)
  {
    bucket_destroy(next);
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (size_t i = 0; i < no_buckets; i++)
  {
    bucket_destroy(ht->buckets[i]);
  }
  free(ht);
}

static entry_t *find_previous_entry_for_key(ioopm_eq_function key_eq_fnc, entry_t *entry, elem_t key)
{
  entry_t **start_pointer = &entry;

  while (entry != NULL)
  {
    if (entry->next == NULL)
    {
      return *start_pointer; // Key not in bucket!! Return dummy
    }
    if (key_eq_fnc(entry->next->key, key))
    {
      return entry;
    }
    entry = entry->next;
  }
  return *start_pointer;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));

  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function key_func, ioopm_eq_function value_func, ioopm_hash_function hash_func)
{
  /// Allocate space for a ioopm_hash_table_t = no_buckets pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  // All buckets get a dummy!
  for (size_t i = 0; i < no_buckets; i++)
  {
    result->buckets[i] = entry_create(ioopm_create_empty_elem(), ioopm_create_empty_elem(), NULL);
  }

  result->key_eq_fn = key_func;
  result->value_eq_fn = value_func;

  if (hash_func == NULL)
  {
    result->hash_function = ioopm_int_hash;
  }
  else
  {
    result->hash_function = hash_func;
  }

  return result;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  int bucket = ht->hash_function(key);
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht->key_eq_fn, ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && ht->key_eq_fn(next->key, key))
  {
    next->value = value;
  }
  else
  {
    entry->next = entry_create(key, value, next);
  }
}

ioopm_option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  if (ht)
  {
    int bucket = ht->hash_function(key);
    entry_t *tmp_prev = find_previous_entry_for_key(ht->key_eq_fn, ht->buckets[bucket], key);

    if (!tmp_prev->next)
    {
      ioopm_option_t result = {.success = false, .value = ioopm_create_empty_elem()};
      return result;
    }
    else
    {
      ioopm_option_t result = {.success = true, .value = tmp_prev->next->value};
      return result;
    }
  }
  return (ioopm_option_t){.success = false};
}

ioopm_option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = ht->hash_function(key);
  entry_t *prev_entry = find_previous_entry_for_key(ht->key_eq_fn, ht->buckets[bucket], key);

  if ((prev_entry == ht->buckets[bucket] && !prev_entry->next) || !ht->key_eq_fn(prev_entry->next->key, key)) // check if the returned values is the dummy and if it's pointing at NULL!  AKA bucket is empty! It also checks if the key is in the bucket
  {
    ioopm_option_t result = {.success = false, .value = ioopm_create_empty_elem()};
    return result;
  }
  else // remove choosen element
  {
    entry_t *removed = prev_entry->next;
    prev_entry->next = removed->next;
    ioopm_option_t result = {.success = true, .value = removed->value};
    free(removed);
    return result;
  }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t result = 0;
  entry_t *current_entry;

  for (size_t bucket = 0; bucket < no_buckets; bucket++)
  {
    current_entry = ht->buckets[bucket];

    while (current_entry->next != NULL)
    {
      current_entry = current_entry->next;
      result += 1;
    }
  }
  return result;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{

  for (int bucket = 0; bucket < no_buckets; bucket++)
  {
    if (ht->buckets[bucket]->next != NULL)
    {
      return false;
    }
  }
  return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < no_buckets; i++)
  {
    bucket_destroy(ht->buckets[i]);
    ht->buckets[i] = entry_create(ioopm_create_empty_elem(), ioopm_create_empty_elem(), NULL);
  }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *key_ll = ioopm_linked_list_create(ht->key_eq_fn);
  size_t no_found = 0;
  entry_t *current_entry;
  for (int i = 0; (i < no_buckets) && (no_found <= size); i++) // ballin
  {
    current_entry = ht->buckets[i]->next; // Jump into bucket
    if (current_entry)                    // Handling empty bucket case
    {
      while (current_entry) // Climb into a bucket
      {
        ioopm_linked_list_append(key_ll, current_entry->key);
        no_found++;
        current_entry = current_entry->next;
      }
    }
  }
  return key_ll;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *value_ll = ioopm_linked_list_create(ht->value_eq_fn);
  size_t no_found = 0;
  entry_t *current_entry;
  for (int i = 0; (i < no_buckets) && (no_found < size); i++) // ballin
  {
    current_entry = ht->buckets[i]->next; // Jump into bucket
    if (current_entry)                    // Handling empty bucket case
    {
      while (current_entry) // Climb into a bucket
      {
        ioopm_linked_list_append(value_ll, current_entry->value);
        no_found++;
        current_entry = current_entry->next;
      }
    }
  }
  return value_ll;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  
  for (size_t i = 0; i < ioopm_hash_table_size(ht); i++)
  {
    bool is_found = false;
    entry_t *current;
    for (size_t i = 0; i < no_buckets; i++) // check every bucket
    {
      current = ht->buckets[i]->next;
      while (current) // every entry in bucket
      {
        is_found = ht->key_eq_fn(current->key, key);
        if (is_found)
        {
          return true;
        }
        current = current->next;
      }
    }
  }

  return false;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  entry_t *current = NULL;
  bool func_bool = false;

  for (size_t i = 0; i < no_buckets; i++) // looks through all buckets
  {
    current = ht->buckets[i]->next;
    while (current != NULL) // looks through all entries in given bucket
    {
      func_bool = ht->value_eq_fn(current->value, value);

      if (func_bool)
      {
        return true;
      }
      current = current->next;
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_hash_predicate func, void *extra)
{
  entry_t *current = NULL;
  bool func_bool = true;

  for (size_t i = 0; i < no_buckets; i++) // looks trough all buckets
  {
    current = ht->buckets[i]->next;
    while (current != NULL) // looks through all entries in given bucket
    {

      func_bool = func(current->key, current->value, extra);
      if (!func_bool)
      {
        return false;
      }
      current = current->next;
    }
  }
  return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_hash_predicate func, void *extra)
{
  entry_t *current = NULL;
  bool func_bool = false;

  for (size_t i = 0; i < no_buckets; i++) // looks through all buckets
  {
    current = ht->buckets[i]->next;
    while (current != NULL) // looks through all entries in given bucket
    {
      func_bool = func(current->key, current->value, extra);

      if (func_bool)
      {
        return true;
      }
      current = current->next;
    }
  }
  return false;
}
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_hash_function apply_fun, void *arg)
{
  entry_t *current = NULL;
  size_t size = ioopm_hash_table_size(ht);
  size_t j = 0;
  for (size_t i = 0; i < no_buckets && j < size; i++) // looks through all buckets
  {
    current = ht->buckets[i]->next;
    while (current != NULL) // looks through all entries in given bucket
    {
      j++;
      apply_fun(current->key, &current->value, arg);
      current = current->next;
    }
  }
}
