#include "back_end.h"
#include <stdbool.h>

bool merchandice_unique(char *name) // TODO
{
    return false;
}

char **ioopm_merchandice_array(ioopm_hash_table_t *HTn) // NEED TO FREE keys
{
    ioopm_list_t *list = ioopm_hash_table_keys(HTn);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    char **keys = calloc(list->size, sizeof(char *));

    for (size_t i = 0; i < list->size; i++)
    {
        keys[i] = ioopm_iterator_current(iter).s;
        if (i < list->size - 1)
        {
            ioopm_iterator_next(iter);
        }
    }
    ioopm_iterator_destroy(iter);
    return keys;
}

ioopm_item_t remove_merchandise_back_end(ioopm_warehouse_t warehouse, char *key) // TODO NEED TO FREE
{
   
    ioopm_item_t *tmp_ptr = ioopm_hash_table_remove(warehouse.HTn, ioopm_str_to_elem(key)).value.p;
    ioopm_item_t tmp = *tmp_ptr;
    size_t size = ioopm_linked_list_size(tmp.llsl);
    for (size_t i = 0; i < size; i++)
    {
        ioopm_hash_table_remove(warehouse.HTsl, ioopm_ptr_to_elem(&tmp.llsl[i]));
    }

    return tmp;
}

ioopm_warehouse_t ioopm_create_warehouse()
{
    ioopm_hash_table_t *HTn = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    ioopm_hash_table_t *HTsl = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    return (ioopm_warehouse_t){.HTn = HTn, .HTsl = HTsl};
}

size_t ioopm_ht_size(ioopm_hash_table_t *HTn)
{
    return ioopm_hash_table_size(HTn);
}

void ioopm_warehouse_destroy(ioopm_warehouse_t warehouse)
{
    ioopm_hash_table_destroy(warehouse.HTn);
    ioopm_hash_table_destroy(warehouse.HTsl);
}