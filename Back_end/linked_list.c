#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * @file simple_linked_list.c
 * @author Elias Castegren
 * @date 1 Sep 2021
 * @brief A simple linked list that implements parts of the interface
 * of `linked_list.h`. */


#define ERR_NULL_List "\nNothing to get! Aborting..."
#define ERR_Out_of_bounds "\nIndex out of bounds! Aborting..."


link_t *link_create(elem_t value, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_fn)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    list->eq_fn = eq_fn;
    return list;
}


void ioopm_fill_linked_list(ioopm_list_t *list, elem_t *values, int length) 
{
    if (list) // Handling NULL list case
    {
        for (int i = 0; i < length; i++)
        {
            ioopm_linked_list_append(list, values[i]);
        }
    }
    else
    {
        perror(ERR_NULL_List);
    }
}



void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    if (list) // Handling NULL list case
    {
        if (list->head) // Handling empty list case
        {
            link_t *current = list->head;
            while (current)
            {
                link_t *tmp = current;
                current = current->next;
                free(tmp);
            }
        }
        free(list);
    }
    else
    {
        perror(ERR_NULL_List);
    }
}


void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    if (list) // Handling NULL list case
    {
        if (list->size == 0)
        {
            list->head = link_create(value, NULL);
        }
        else
        {
            link_t *current = list->head;
            while (current->next)
            {
                current = current->next;
            }
            current->next = link_create(value, NULL);
        }
        list->size++;
    }
    else
    {
        perror(ERR_NULL_List);
    }
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    if (list) // Handling NULL list case
    {
        list->head = link_create(value, list->head);
        list->size++;
    }
    else
    {
        perror(ERR_NULL_List);
    }
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
    if (list) // Handling NULL list case
    {
        if (index <= list->size - 1) // Handling out of bounds case
        {
            if (index == 0)
            {
                list->head = link_create(value, list->head);
                list->size += 1;
            }
            else
            {
                link_t *current = list->head;
                for (size_t i = 1; i < index; i++)
                {
                    current = current->next;
                }
                current->next = link_create(value, current->next);
                list->size += 1;
            }
        }
        else
        {
            perror(ERR_Out_of_bounds);
        }
    }
    else
    {
        perror(ERR_NULL_List);
    }
}

static elem_t linked_list_remove(ioopm_list_t *list, size_t index)
{
    if (index == 0) // no failsafes needed (this is essentially helper function)
    {
        link_t *tmp = list->head;
        elem_t value = tmp->value;
        list->head = tmp->next;
        free(tmp);
        list->size--;
        return value;
    }
    else
    {
        link_t *prev = list->head;
        link_t *current = prev->next;
        for (size_t i = 1; i < index; i++)
        {
            prev = current;
            current = current->next;
        }
        prev->next = current->next;
        elem_t value = current->value;
        free(current);
        list->size--;
        return value;
    }
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
    if (list) // Handling NULL list case
    {
        if (!list->head || list->size <= index) // Handling empty and out of of bounds cases
        {
            if (list->size < index)
            {
                perror(ERR_Out_of_bounds);
                return ioopm_create_empty_elem();
            }
            else
            {
                perror(ERR_NULL_List);
                return ioopm_create_empty_elem();
            }
        }

        return linked_list_remove(list, index);
    }
    else
    {
        perror(ERR_NULL_List);
        return ioopm_create_empty_elem();
    }
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
    if (list) // Handling NULL list case
    {
        if (list->size <= index) // Handels out of bounds Case
        {
            perror(ERR_Out_of_bounds);
            return ioopm_create_empty_elem();
        }

        link_t *current = list->head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }

        return current->value;
    }
    else
    {
        perror(ERR_NULL_List);
        return ioopm_create_empty_elem();
    }
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    if (list) // Handling NULL list case
    {
        link_t *cursor = list->head;
        while (cursor)
        {
            if (list->eq_fn(cursor->value, element))
            {
                return true;
            }
            cursor = cursor->next;
        }
    }
    perror(ERR_NULL_List);
    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    if (list) // Handling NULL list case
    {
        return list->size;
    }
    else
    {
        perror(ERR_NULL_List);
        return 0;
    }
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    if (list) // Handling NULL list case
    {
        return (list->size == 0);
    }
    else
    {
        return true;
    }
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    if (list) // Handling NULL list case
    {
        if (list->head) // Handling empty list case
        {
            size_t size = ioopm_linked_list_size(list);
            for (size_t i = 0; i < size; i++)
            {
                linked_list_remove(list, 0);
            }
        }
        else
        {
            perror(ERR_NULL_List);
        }
    }
    else
    {
        perror(ERR_NULL_List);
    }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_list_predicate prop, void *extra)
{
    if (list) // Handling NULL list case
    {
        link_t *current = list->head;
        bool func_bool = true;
        size_t index = 0;
        while (current)
        {
            func_bool = prop(index, current->value, extra);

            if (!func_bool)
            {
                return false;
            }
            current = current->next;
            index++;
        }
        if (!list->head)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        perror(ERR_NULL_List);
        return false;
    }
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_list_predicate prop, void *extra)
{
    if (list) // Handling NULL list case
    {
        link_t *current = list->head;
        bool func_bool = false;
        size_t index = 0;

        while (current)
        {
            func_bool = prop(index, current->value, extra);

            if (func_bool)
            {
                return true;
            }
            current = current->next;
            index++;
        }
        if (!list->head)
        {
            return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        perror(ERR_NULL_List);
        return false;
    }
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_list_function fun, void *extra)
{
    if (list) // Handling NULL list case
    {
        if (list->head) // Handling empty list case
        {
            link_t *current = list->head;
            size_t index = 0;
            while (current)
            {
                fun(index, &(current->value), extra);
                current = current->next;
                index++;
            }
        } 
        else
        {
            perror(ERR_NULL_List);
        }
    }
    else
    {
        perror(ERR_NULL_List);
    }
}

