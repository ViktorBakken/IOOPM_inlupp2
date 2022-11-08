
#include "items_db_back_end.h"

#define ERR_non_unique "Error item is non unique!"

ioopm_item_t *ioopm_choose_item_from_list_backend(ioopm_hash_table_t *HTn, size_t index)
{
    if (HTn)
    {
        size_t size = ioopm_hash_table_size(HTn);
        if (index < size)
        {
            string *item = ioopm_item_array(HTn);
            
            qsort(item, size, sizeof(string), cmpstringp); // taken from freq-count.c

            ioopm_item_t *tmp_item = (ioopm_hash_table_lookup(HTn, ioopm_str_to_elem(item[index])).value).p;

            ioopm_item_list_destroy(item, size);

            return tmp_item;
        }
    }
    return NULL;
}

ioopm_item_t *ioopm_make_item_backend(string name, string descr, size_t price) // TODO free name, descr, llsl and item
{
    ioopm_list_t *llsl = ioopm_linked_list_create(ioopm_elem_str_eq);
    ioopm_item_t *item = calloc(1, sizeof(ioopm_item_t));

    *item = (ioopm_item_t){.name = name, .desc = descr, .price = price, .llsl = llsl};
    return item;
}

bool ioopm_add_item(ioopm_hash_table_t *HTn, ioopm_item_t *new_item)
{
    if (ioopm_item_name_unique(HTn, new_item->name))
    {
        ioopm_hash_table_insert(HTn,
                                ioopm_str_to_elem(new_item->name),
                                ioopm_ptr_to_elem(new_item));
        return true;
    }
    perror(ERR_non_unique);
    return false;
}