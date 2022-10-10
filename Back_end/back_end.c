#include "back_end.h"
#include <stdbool.h>

bool merchandice_unique(char *name) // TODO
{
    return false;
}

char **ioopm_merchandice_array(ioopm_hash_table_t *HTn)
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

ioopm_warehouse_t ioopm_create_warehouse()
{
    // ioopm_hash_table_t *HTn = ioopm_hash_table_create(ioopm_elem_str_eq,);
    // ioopm_hash_table_t *HTsl = ioopm_hash_table_create();
    // return (ioopm_warehouse_t){.HTn = HTn, .HTsl = HTsl};
}