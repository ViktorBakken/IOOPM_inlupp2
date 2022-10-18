#pragma once
#include "Generic_func_Data_types/common.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

/// @brief Checks if a merchandise already exist
/// @param name the name of the item to be checked
bool merchandice_unique(ioopm_hash_table_t *HTn, char *name);

/// @brief adds a item to a hashtable
/// @param HTn given hashtable
/// @param new_item the item to be added
void ioopm_add_merchandise_backend(ioopm_hash_table_t *HTn, ioopm_item_t *new_item);

/// @brief creates a item_t from given input
/// @param HTn Given hashtable
/// @param index the number of the wanted item_t
/// @return A item_t containing the inputs
ioopm_item_t ioopm_choose_item_from_list_backend(ioopm_hash_table_t *HTn, size_t index);

/// @brief convert a hash table to an array
/// @param llsl is the linked list to be converted
/// @return array containing the converted linked list
string *ioopm_llsl_array(ioopm_list_t *llsl);

/// @brief creates a item_t from given input
/// @param name The name of the item
/// @param descr The description of the item
/// @param price The price of the item
/// @return A item_t containing the inputs
ioopm_item_t make_item_backend(string name, string descr, size_t price);

/// @brief convert a hash table to an array
/// @param HTn is the hashtable to be converted
/// @return array containing the keys of the hash table
string *ioopm_merchandice_array(ioopm_hash_table_t *HTn);

/// @brief Creates a warehouse
/// @return a empty warehouse
/// @warning Needs to be destroyed
ioopm_warehouse_t ioopm_create_warehouse();

/// @brief Gives the size of a given hashtable
/// @param HTn the given hashtable
/// @return The amount of items in the given hashtable
size_t ioopm_ht_size(ioopm_hash_table_t *HTn);

/// @brief remove a specific merchandise from warehouse
/// @param warehouse is the warehouse where the item is removed
/// @param key the name of the item to be removed
/// @return the item that is removed
/// @warning the returend item needs to be freed
ioopm_item_t ioopm_remove_merchandise(ioopm_warehouse_t *warehouse, string key);

/// @brief destroys a given warehouse
/// @param warehouse the warehouse to be destroyed
void ioopm_warehouse_destroy(ioopm_warehouse_t warehouse);

ioopm_hash_table_t *create_cart_backend(void);

void remove_cart_backend(ioopm_hash_table_t *cart);

void ioopm_add_to_cart(ioopm_hash_table_t *cart, ioopm_item_t *item, size_t amount);
