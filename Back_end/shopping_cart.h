#pragma once

#include "back_end.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "../db.h"

typedef struct cart ioopm_shopping_cart_t;

ioopm_shopping_cart_t create_cart(int id);

void remove_cart(ioopm_shopping_cart_t *cart);

void add_to_cart(ioopm_warehouse_t warehouse, ioopm_shopping_cart_t *cart, int index, int amount);

void remove_from_cart(ioopm_shopping_cart_t *cart);

int calculate_cost(ioopm_shopping_cart_t *cart, ioopm_warehouse_t warehouse);

void checkout(ioopm_shopping_cart_t *cart, ioopm_warehouse_t *warehouse);
