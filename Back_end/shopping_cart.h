#pragma once

#include "back_end.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "../db.h"

typedef struct cart ioopm_shopping_cart_t;

void create_cart(ioopm_hash_table_t *carts, int id);

void remove_cart(ioopm_shopping_cart_t *cart);

void ioopm_destroy_cart(elem_t unused_key, elem_t *value, void *unused_extra);

void add_to_cart(ioopm_hash_table_t *cart, size_t amount, ioopm_item_t *item);

void remove_from_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item_rem);

int calculate_cost(ioopm_shopping_cart_t *cart, ioopm_warehouse_t warehouse);

void checkout(ioopm_shopping_cart_t *cart, ioopm_warehouse_t *warehouse);
