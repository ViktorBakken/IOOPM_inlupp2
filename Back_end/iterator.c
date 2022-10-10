#include "iterator.h"
#include <stdlib.h>
#include <stdio.h>

#define ERR_null_input "\nInvalid input (NULL pointer)! Aborting..."
#define ERR_null_next "\nNext iterator is NULL! Aborting..."


bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (iter) // Handeling NULL iter case
    {
        if (iter->current) // Handeling empty iter case
        {
            return iter->current->next != NULL;
        }
    }
    perror(ERR_null_input);
    return false;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    if (iter) // Handeling NULL iter case
    {
        if (iter->current) // Handeling empty iter case
        {
            if (iter->current->next) // End of list case
            {
                iter->current = iter->current->next;
                return (iter->current->value);
            }
            else
            {
                iter->current = iter->current->next;
                perror(ERR_null_next);
                return ioopm_create_empty_elem();
            }
        }
        else
        {
            perror(ERR_null_input);
            return ioopm_create_empty_elem();
        }
    }
    else
    {
        perror(ERR_null_input);
        return ioopm_create_empty_elem();
    }
}

//remove är uppenbarligen cringe
// // so many ifs. not nice
// int ioopm_iterator_remove(ioopm_list_iterator_t *iter)
// {
//     if (iter) // Handels NULL iter case
//     {
//         if (iter->current) // handels empty iter case
//         {
//             if (iter->list) // Handeling NULL list case
//             {
//                 if (iter->list->head) // Handeling empty list case
//                 {
//                     if (iter->current->next)
//                     {
//                         if (iter->current == iter->list->head) // if its the head then we also change the head pointer in the list
//                         {
//                             iter->list->head = iter->current->next;
//                         }
//                         int retur = iter->current->value;
//                         link_t *old_link = iter->current->next;
//                         iter->current->value = iter->current->next->value;
//                         iter->current->next = iter->current->next->next;
//                         free(old_link); // varning - om man sparat pointer till next så kan det gå illa
//                         iter->list->size--;
//                         return retur;
//                     }
//                     else
//                     {
//                         if (iter->current == iter->list->head) // if its the head then we also change the head pointer to null (because its the only element)
//                         {
//                             iter->list->head = NULL;
//                         }
//                         int retur = iter->current->value;
//                         free(iter->current);
//                         iter->current = NULL;
//                         iter->list->size--;
//                         return retur;
//                     }
//                 }
//             }
//         }
//     }
//     perror(ERR_null_input);
//     return -1;
// }

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    if (iter) // Handeling NULL iter case
    {
        if (iter->current) // Handeling empty list case or other iter current = NULL case
        {
            link_t *old_link = calloc(1, sizeof(link_t));
            old_link->value = iter->current->value;
            old_link->next = iter->current->next;

            iter->current->next = old_link;
            iter->current->value = element;
            iter->list->size++;

            // if (old_link == iter->list->head) //ärligt talat vetefan varför detta är ett jävla problem??
            // {
            //     iter->list->head = iter->current;
            // }
        }
        else if (iter->list) // Handeling NULL list case
        {
            if (!iter->list->head) // if list head is null (i.e list empty case) head needs to be set to the new head
            {
                link_t *new_head = calloc(1, sizeof(link_t));
                new_head->value = element;
                new_head->next = NULL;
                iter->current = new_head;
                iter->list->head = new_head;
                iter->list->size++;
            }
            else
            {
                perror(ERR_null_input); // if current is null but the list head isnt. (means that iter is NULL when it shouldnt be. Use iterator_reset)
            }
        }
        else
        {
            perror(ERR_null_input);
        }
    }
    else
    {
        perror(ERR_null_input);
    }
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    bool success = false;
    if (iter) // Handeling NULL iter case
    {
        if (iter->list) // Handeling nonexistent list case
        {
            success = true;
            iter->current = iter->list->head;
        }
    }
    if (success == false)
    {
        perror(ERR_null_input);
    }
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    if (iter) // Handeling NULL iter case
    {
        if (iter->current) // Handeling empty iter case
        {
            return (iter->current->value);
        }
    }
    perror(ERR_null_input);
    return ioopm_create_empty_elem();
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    if (iter) // Handeling NULL iter case
    {
        free(iter);
    }
    else
    {
        perror(ERR_null_input);
    }
}

/*
gcc iterator.c iterator_test.c linked_list.c -lcunit && ./a.out
*/