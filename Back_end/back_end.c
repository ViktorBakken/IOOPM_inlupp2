
#include "back_end.h"
#include <stdbool.h>
#include <stdlib.h>

void ioopm_merchandise_list_destroy(string *merchandise, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
        free(merchandise[i]);
    }
    free(merchandise);
}

static void remove_from_stock(ioopm_hash_table_t *HTsl, ioopm_list_t *llsl)
{
    size_t size = ioopm_linked_list_size(llsl);
    for (size_t i = 0; i < size; i++)
    {
        ioopm_hash_table_remove(HTsl, ioopm_linked_list_get(llsl, i));
    }
}

// Merchandise
bool merchandice_unique(ioopm_hash_table_t *HTn, string name)
{
    return !ioopm_hash_table_has_key(HTn, ioopm_str_to_elem(name));
}

bool cart_unique(ioopm_hash_table_t *cart, int id)
{
    return !ioopm_hash_table_has_key(cart, ioopm_int_to_elem(id));
}

bool shelf_unique(ioopm_hash_table_t *HTsl, string shelf_name)
{
    return !ioopm_hash_table_has_key(HTsl, ioopm_str_to_elem(shelf_name));
}

string *ioopm_merchandice_array(ioopm_hash_table_t *HTn) // TODO NEED TO FREE keys
{
    ioopm_list_t *list = ioopm_hash_table_keys(HTn);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    string *keys = calloc(list->size, sizeof(string));

    for (size_t i = 0; i < list->size; i++)
    {
        keys[i] = strdup(ioopm_iterator_current(iter).s);
        if (i < list->size - 1)
        {
            ioopm_iterator_next(iter);
        }
    }
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iter);
    return keys;
}

ioopm_item_t ioopm_remove_merchandise(ioopm_warehouse_t *warehouse, string key)
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

void ioopm_add_merchandise_backend(ioopm_hash_table_t *HTn, ioopm_item_t *new_item)
{
    ioopm_hash_table_insert(HTn,
                            ioopm_str_to_elem(new_item->name),
                            ioopm_ptr_to_elem(new_item));
}

// Hash_table
size_t ioopm_ht_size(ioopm_hash_table_t *HTn)
{
    return ioopm_hash_table_size(HTn);
}

ioopm_item_t *ioopm_choose_item_from_list_backend(ioopm_hash_table_t *HTn, size_t index)
{
    int size = ioopm_hash_table_size(HTn);
    string *merchandise = ioopm_merchandice_array(HTn);
    qsort(merchandise, size, sizeof(string), cmpstringp); // taken from freq-count.c

    ioopm_item_t *tmp_item = (ioopm_hash_table_lookup(HTn, ioopm_str_to_elem(merchandise[index])).value).p;

    ioopm_merchandise_list_destroy(merchandise, size);

    return tmp_item;
}

// Linked l
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

ioopm_item_t *make_item_backend(string name, string descr, size_t price) //TODO free name, descr, llsl and item
{
    ioopm_list_t *llsl = ioopm_linked_list_create(ioopm_elem_str_eq);
    ioopm_item_t *item = calloc(1, sizeof(ioopm_item_t));

    *item = (ioopm_item_t){.name = name, .desc = descr, .price = price, .llsl = llsl};
    return item;
}

// Warehouse
ioopm_warehouse_t ioopm_create_warehouse()
{
    ioopm_hash_table_t *HTn = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    ioopm_hash_table_t *HTsl = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    return (ioopm_warehouse_t){.HTn = HTn, .HTsl = HTsl};
}

void destroy_cart_backend(ioopm_hash_table_t *cart)
{
    ioopm_hash_table_destroy(cart);
}

void ioopm_destroy_cart_list(ioopm_hash_table_t *all_carts)
{
    ioopm_hash_table_apply_to_all(all_carts, ioopm_destroy_cart, NULL);
    ioopm_hash_table_destroy(all_carts);
}

void destroy_shelf(size_t unused_index, elem_t *value, void *unused_extra)
{
    (void) unused_index;
    (void) unused_extra;
    free(value->s);
    // free(value);
}

void destroy_item(elem_t unused_key, elem_t *value, void *unused_extra)
{
    (void)unused_extra;
    (void)unused_key;
    ioopm_item_t *item_ptr = value->p;
    free(item_ptr->name);
    free(item_ptr->desc);
    ioopm_linked_list_apply_to_all(item_ptr->llsl, destroy_shelf, NULL);
    ioopm_linked_list_destroy(item_ptr->llsl);
    free(item_ptr);
}


void ioopm_warehouse_destroy(ioopm_warehouse_t *warehouse) // TODO free strings, DONE?(ger valgrind error?)
{
    ioopm_hash_table_apply_to_all(warehouse->HTn, destroy_item, NULL);
    ioopm_hash_table_destroy(warehouse->HTn);
    ioopm_hash_table_destroy(warehouse->HTsl); 
}

// Cart
void *choose_cart(ioopm_hash_table_t *all_carts, int id)
{
    ioopm_option_t output = ioopm_hash_table_lookup(all_carts, ioopm_int_to_elem(id));
    return output.success ? output.value.p : NULL;
}

ioopm_hash_table_t *create_cart_backend(void)
{
    ioopm_hash_table_t *cart = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    return cart;
}

ioopm_hash_table_t *ioopm_create_cart_list(void)
{
    return ioopm_hash_table_create(ioopm_elem_int_eq, NULL, ioopm_int_hash);
}

void ioopm_new_cart_backend(ioopm_hash_table_t *all_carts, void *new_cart, int id)
{
    ioopm_hash_table_insert(all_carts, ioopm_int_to_elem(id), ioopm_ptr_to_elem(new_cart));
}

void remove_cart_backend(ioopm_hash_table_t *cart)
{
    ioopm_hash_table_destroy(cart);
}

void ioopm_remove_from_cart(ioopm_hash_table_t *cart, string name)
{

    ioopm_hash_table_remove(cart, ioopm_str_to_elem(name));
}
