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
/// @return 
void ioopm_add_merchandise_backend(ioopm_hash_table_t *HTn, ioopm_item_t *new_item);

/// @brief convert a hash table to an array
/// @param llsl is the linked list to be converted
/// @return array containing the converted linked list
string *ioopm_llsl_array(ioopm_list_t *llsl);

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

