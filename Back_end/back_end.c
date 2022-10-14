#include "back_end.h"
#include <stdbool.h>

bool merchandice_unique(ioopm_hash_table_t *HTn, string name)
{
    return ioopm_hash_table_has_key(HTn, ioopm_str_to_elem(name));
}

ioopm_item_t ioopm_choose_item_from_list(ioopm_hash_table_t *HTn)
{
    size_t index = ask_question_int("Whiche item?"); //TODO add more security
    string *merchandise = ioopm_merchandice_array(HTn);
    int size = ioopm_hash_table_size(HTn);
    qsort(merchandise, size, sizeof(string), cmpstringp); // taken from freq-count.c

    void *tmp = (ioopm_hash_table_lookup(HTn, ioopm_str_to_elem(merchandise[index])).value).p;
    ioopm_item_t *tmp_item = tmp;
    return *tmp_item;
}

string *ioopm_llsl_array(ioopm_list_t *llsl) // NEED TO FREE keys
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(llsl);
    char **keys = calloc(llsl->size, sizeof(char *));

    for (size_t i = 0; i < llsl->size; i++)
    {
        keys[i] = ioopm_iterator_current(iter).s;
        if (i < llsl->size - 1)
        {
            ioopm_iterator_next(iter);
        }
    }
    ioopm_iterator_destroy(iter);
    return keys;
}

string *ioopm_merchandice_array(ioopm_hash_table_t *HTn) // NEED TO FREE keys
{
    ioopm_list_t *list = ioopm_hash_table_keys(HTn);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    string *keys = calloc(list->size, sizeof(string));

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

static void remove_from_stock(ioopm_hash_table_t *HTsl, ioopm_list_t *llsl)
{
    size_t size = ioopm_linked_list_size(llsl);
    for (size_t i = 0; i < size; i++)
    {
        ioopm_hash_table_remove(HTsl, ioopm_linked_list_get(llsl, i));
    }
    
}

ioopm_item_t ioopm_remove_merchandise(ioopm_warehouse_t *warehouse, string key) // TODO NEED TO FREE
{
    if (merchandice_unique(warehouse->HTn, key))
    {
        ioopm_item_t *tmp_ptr = ioopm_hash_table_remove(warehouse->HTn, ioopm_str_to_elem(key)).value.p;
        ioopm_item_t tmp = *tmp_ptr;
        size_t size = ioopm_linked_list_size(tmp.llsl);
        for (size_t i = 0; i < size; i++)
        {
            ioopm_hash_table_remove(warehouse->HTsl, ioopm_ptr_to_elem(&tmp.llsl[i]));
        }
        remove_from_stock(warehouse->HTsl, tmp.llsl);
        return tmp;
    }
    return empty_item_t;
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