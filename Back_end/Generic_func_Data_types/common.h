#pragma once 
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

/**
 * @file common.h
 * @author Egon Henriksen och Viktor Bakken
 * @date 13 Sep 2022
 * @brief Common data types for hash_table, linked_list and iterator.
 */

// elem_t def
typedef union elem elem_t;
union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  char *s;
  void *p;
};

// define macros
#define no_buckets 17

// Empty elem_t element
/// @brief Creates an empty elem_t element
/// @returns An empty elem_t element
elem_t ioopm_create_empty_elem();

/// @brief Checks if elem_t  element is empty
bool ioopm_elem_is_empty(elem_t elem);

// Predicate functions: For use in all/any functions for list and hash table respectively
typedef bool (*ioopm_list_predicate)(size_t index, elem_t value, void *extra);
typedef bool (*ioopm_hash_predicate)(elem_t key, elem_t value, void *extra);

// Apply functions: For use in apply_all functions for list and hash table respectively
typedef void (*ioopm_apply_hash_function)(elem_t key, elem_t *value, void *extra);
typedef void (*ioopm_apply_list_function)(size_t index, elem_t *value, void *extra);

// Eq function: (see commmon.c) used in various functions to compare the values of two elements.
// Depending on which you use when creating list/hashtable dictates which types it uses.
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);

// Hashtable also needs hash function for the key value's type.
typedef int (*ioopm_hash_function)(elem_t key);
typedef elem_t (*to_elem_func)(void *ptr);

// Compare elements func
/// @brief Compares the ints of two elem_t elements. Used for hash_table_create
/// @param elem1 first elem_t with an int
/// @param elem2 second elem_t with an int
/// @returns true if they are equal, false if not
bool ioopm_elem_int_eq(elem_t elem1, elem_t elem2);

/// @brief Compares the strings of two elem_t elements. Used for hash_table_create
/// @param elem1 first elem_t with a string
/// @param elem2 second elem_t with a string
/// @returns true if they are equal, false if not
bool ioopm_elem_str_eq(elem_t elem1, elem_t elem2);

/// @brief Compares the bools of two elem_t elements. Used for hash_table_create
/// @param elem1 first elem_t with a bool
/// @param elem2 second elem_t with a bool
/// @returns true if they are equal, false if not
bool ioopm_elem_bool_eq(elem_t elem1, elem_t elem2);

/// @brief Compares the floats of two elem_t elements. Used for hash_table_create
/// @param elem1 first elem_t with a float
/// @param elem2 second elem_t with a float
/// @returns true if they are equal, false if not
bool ioopm_elem_float_eq(elem_t elem1, elem_t elem2);

/// @brief Compares the unsigned ints of two elem_t elements. Used for hash_table_create
/// @param elem1 first elem_t with an unsigned int
/// @param elem2 second elem_t with an unsigned int
/// @returns true if they are equal, false if not
bool ioopm_elem_uint_eq(elem_t elem1, elem_t elem2);

// Hashing func
/// @brief Hashes the string from an elem_t element over no_buckets
/// @param key key to hash
/// @returns int made from the hash
int ioopm_string_hash(elem_t key);

/// @brief Hashes the int from an elem_t element over no_buckets
/// @param key key to be hashed
/// @returns int made from the hash
int ioopm_int_hash(elem_t key);

/// @brief Hashes the unsigned int from an elem_t element over no_buckets
/// @param key key to hash
/// @returns int made from the hash
int ioopm_uint_hash(elem_t key);

// Convert to elem_t
/// @brief Converts an int to a elem_t element
/// @param i the int to be converted
/// @returns an elem_t element with the value inserted
elem_t ioopm_int_to_elem(int i);

/// @brief Converts an void pointer to a elem_t element
/// @param ptr the pointer to be converted
/// @returns an elem_t element with the value inserted
elem_t ioopm_ptr_to_elem(void *ptr);

/// @brief Converts a string pointer to a elem_t element
/// @param str the string pointer to be converted
/// @returns an elem_t element with the value inserted
elem_t ioopm_str_to_elem(char *str);

/// @brief Converts an unsidint to a elem_t element
/// @param uint the unsigned int to be converted
/// @returns an elem_t element with the value inserted
elem_t ioopm_uint_to_elem(unsigned int uint);

// Struct/Union def
// Hash_Table
typedef struct entry entry_t;
struct entry
{
  elem_t key;    // holds the key
  elem_t value;  // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

typedef struct hash_table ioopm_hash_table_t;
struct hash_table
{
  entry_t *buckets[no_buckets];
  ioopm_eq_function value_eq_fn;
  ioopm_eq_function key_eq_fn;
  ioopm_hash_function hash_function;
};

// linked_list
typedef struct link link_t;
struct link
{
  elem_t value;
  struct link *next;
};

typedef struct list ioopm_list_t;
struct list
{
  link_t *head;
  size_t size;
  ioopm_eq_function eq_fn;
};

typedef struct iterator ioopm_list_iterator_t;
struct iterator
{
  link_t *current;
  ioopm_list_t *list;
};
