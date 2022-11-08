#pragma once
#include "linked_list.h"
#include "Generic_func_Data_types/common.h"

/**
 * @file iterator.h
 * @author Egon Henriksen och Viktor Bakken
 * @date 13 Sep 2022
 * @brief Simple iterator.
 */


/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element
/// @exception when iter or iter->current is NULL, will return false
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
/// @return the next element
/// @exception if iter or iter->current is NULL, returns -1. if iter->current->next is NULL, iter->current becomes null and -1 is returned
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter);

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
/// @exception if iter or iter->current is NULL, does nothing
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
/// @exception if iter, iter->current or iter->list is NULL, does nothing. If iter->list->head is NULL current becomes NULL.
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
/// @exception when iter or iter->current is NULL, will return false
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
/// @exception when iter or iter->current is NULL, will return false
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);