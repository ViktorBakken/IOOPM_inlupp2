#pragma once

#include "back_end.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "../db.h"

typedef struct cart ioopm_shopping_cart_t;

/// @brief The id of a shopping cart
/// @param cart Given shopping cart
/// @return The id
int ioopm_cart_id(ioopm_shopping_cart_t *cart);

/// @brief
/// @param all_carts
/// @param id
void create_cart(ioopm_hash_table_t *all_carts, int id);

/// @brief
/// @param cart
void remove_cart(ioopm_shopping_cart_t *cart);

/// @brief
/// @param unused_key
/// @param value
/// @param unused_extra
void ioopm_destroy_cart(elem_t unused_key, elem_t *value, void *unused_extra);

/// @brief
/// @param cart
/// @param amount
/// @param item
void add_to_cart(ioopm_shopping_cart_t *cart, size_t amount, ioopm_item_t *item);

/// @brief Prints all items and their respective amount from given cart
/// @param cart The cart to be printed
void ioopm_list_items_in_cart(ioopm_shopping_cart_t *cart);

/// @brief Removes an item from cart
/// @param cart The cart from wich the item shall be removed from
/// @param item_rem The item to be removed
void remove_from_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item_rem);

/// @brief Calculates the cost of the items in given cart
/// @param cart The cart from which the cost shall be calculated
/// @return The cost of all items in cart
int calculate_cost(ioopm_shopping_cart_t *cart);

/// @brief Checkouts a cart
/// @param cart The cart to be checkedout
/// @param HTsl The shelf hash table
/// @return Boolean value for if the operation succeded
bool checkout(ioopm_shopping_cart_t *cart, ioopm_hash_table_t *HTsl);
