#pragma once/// @brief Insert at the end of a linked list in O(1) time
#include <stdbool.h>
#include "Generic_func_Data_types/common.h"




/// @brief Fills a linked list with an array of values 
/// @param list the list that will be filled
/// @param values is the array that will fill into list
/// @param length the length of the array
/// @exception A NULL list is ignored 
/// @bug if length =/= length(values) it can cause the program to crash
void ioopm_fill_linked_list(ioopm_list_t *list, elem_t *values, int length);

/// @brief Creates a new empty list
/// @param eq_fn a function that is used for checking if two elements are equal
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_fn);

/// @brief Tear down the linked list and return all its memory (but not the memory of any pointer elements that may be on the heap)
/// @param list the list to be destroyed
/// @exception When provided a null pointer list will do nothing
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
/// @exception When provided a null pointer list will do nothing
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
/// @exception When provided a null pointer list will do nothing
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
/// @exception When provided a null pointer list, or an index out of range will do nothing
void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @return the element removed
/// @exception When list is NULL, empty or index is out of bounds an empty element is returned, 
elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list in question
/// @param index the position in the list
/// @return the element at the given position
/// @exception When list is NULL or index is out of bounds an empty element is returned, 
elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index);

/// @brief Test if an element is in the list using the eq_function of the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
/// @exception When list is NULL false is returned
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
/// @exception When provided a null pointer list will return false
size_t ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
/// @exception When provided a null pointer list or empty list will return true
bool ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
/// @exception When provided a null pointer list will do nothing
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
/// @exception When provided a null pointer list or empty list will return false
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_list_predicate prop, void *extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
/// @exception A NULL pointer list or empty list will return false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_list_predicate prop, void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
/// @exception A NULL or empty list is ignored 
void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_list_function fun, void *extra);

/// @brief Create an iterator for a given list
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);