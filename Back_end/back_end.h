#pragma once
#include "Generic_func_Data_types/common.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

/// @brief Checks if a merchandise already exist
/// @param name the name of the item to be checked
bool merchandice_unique(char *name);

char **ioopm_merchandice_array(ioopm_hash_table_t *HTn);

ioopm_warehouse_t ioopm_create_warehouse();

void add_merchandise(ioopm_warehouse_t *warehouse, string name, string description, size_t price);

