#pragma once
#include "Generic_func_Data_types/common.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

/// @brief Checks if a merchandise already exist
/// @param name the name of the item to be checked
bool merchandice_unique(ioopm_hash_table_t *HTn, char *name);

/// @brief convert a hash table to an array
/// @param llsl is the linked list to be converted
/// @return array containing the converted linked list
string *ioopm_llsl_array(ioopm_list_t *llsl);

/// @brief convert a hash table to an array
/// @param HTn is the hashtable to be converted
/// @return array containing the keys of the hash table
string *ioopm_merchandice_array(ioopm_hash_table_t *HTn);

ioopm_warehouse_t ioopm_create_warehouse();

size_t ioopm_ht_size(ioopm_hash_table_t *HTn);

ioopm_item_t ioopm_remove_merchandise(ioopm_warehouse_t *warehouse, string key);

void ioopm_warehouse_destroy(ioopm_warehouse_t warehouse);

