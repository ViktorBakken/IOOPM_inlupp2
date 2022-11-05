#pragma once

#include "db_back_end.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
// #include "../db.h"

typedef struct cart ioopm_shopping_cart_t;

/// @brief obtains the id of a shopping cart
/// @param cart a given shopping cart
/// @return The id
int ioopm_cart_id(ioopm_shopping_cart_t *cart);

/// @brief creates a new empty cart in the list of all carts
/// @param all_carts the list of all carts
/// @param id the identification of that cart
void create_cart(ioopm_hash_table_t *all_carts, int id);


/// @brief makes a  of all carts
/// @return a representation of a list of all the carts
ioopm_hash_table_t *ioopm_create_cart_list(void);

/// @brief destroy the list of carts(free:ing all allocations)
/// @param all_carts the list of carts
void ioopm_destroy_cart_list(ioopm_hash_table_t *all_carts);

/// @brief removes a cart from the list of all carts
/// @param cart a given cart
void ioopm_destroy_cart(ioopm_shopping_cart_t *cart);

/// @brief Delete a cart and free its memory.
/// @param unused_key a irrelevant argument to the function
/// @param value a pointer to the carts items
/// @param unused_extra a irrelevant argument to the function
void destroy_cart_hashfunc(elem_t unused_key, elem_t *value, void *unused_extra);

/// @brief ability to choose a cart
/// @param all_carts is the "list" of all_carts to list items from one specefic
/// @param id the id of the cart choosen
void *choose_cart(ioopm_hash_table_t *all_carts, int id);

/// @brief Obtains amount of a speceifc item in a cart
/// @param cart the cart that potentially has the item
/// @param item the item to check the amount of
/// @return the amount of a specefic item in a cart
int ioopm_amount_items_in_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item);

/// @brief add a given item to a specific cart with a certain amount
/// @param cart a given cart
/// @param item is a given item
void ioopm_add_to_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item, size_t amount);

/// @brief Checks if a given item is in a given shopping cart
/// @param cart The given shopping cart
/// @param item The given item 
/// @return Boolean value if the operations succedes
bool ioopm_is_in_shopping_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item);

/// @brief Prints all items and their respective amount from given cart
/// @param cart The cart to be printed
void ioopm_list_items_in_cart(ioopm_shopping_cart_t *cart);

/// @brief Removes an item from cart
/// @param cart The cart from which the item shall be removed from
/// @param item_rem The item to be removed
void ioopm_remove_from_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item_rem);

/// @brief Calculates the cost of the items in given cart
/// @param cart The cart from which the cost shall be calculated
/// @return The cost of all items in cart
int calculate_cost(ioopm_shopping_cart_t *cart);

/// @brief Checkouts a cart
/// @param cart The cart to be checkedout
/// @param HTsl The shelf hash table
/// @return Boolean value for if the operation succeded
bool checkout(ioopm_shopping_cart_t *cart, ioopm_hash_table_t *HTsl);

/// @brief Checks if a cart is unique
/// @param cart a cart to be checked if it is unique
/// @param id the 
/// @return true or false depending on if its unique or not
bool cart_unique(ioopm_hash_table_t *all_carts, int id);
