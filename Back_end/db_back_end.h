#pragma once
#include "../db.h"
#include "Generic_func_Data_types/store_specific_data_types.h"

#include <stdbool.h>
#include <stdlib.h>


/// @brief creates a item_t from given input
/// @param name The name of the item
/// @param descr The description of the item
/// @param price The price of the item
/// @return A item_t containing the inputs
ioopm_item_t *make_item_backend(string name, string descr, size_t price);

/// @brief creates a item_t from given input
/// @param HTn Given hashtable
/// @param index the number of the wanted item_t
/// @return A item_t containing the inputs
ioopm_item_t *ioopm_choose_item_from_list_backend(ioopm_hash_table_t *HTn, size_t index);

/// @brief Adds a new item to the warehouse with a name, description and price
/// @param HTn the current item's in the warehouse
/// @return true if the item is unique(and adds it), other false(not add it).
bool ioopm_add_item(ioopm_hash_table_t *HTn, ioopm_item_t *new_item);