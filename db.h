#pragma once
#include "Back_end/db_back_end.h"

#include "Back_end/Generic_func_Data_types/utils.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"

#include "Back_end/hash_table.h"
#include "Back_end/linked_list.h"
#include "Back_end/iterator.h"

#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// /// @brief checks if a shelf's location is unique
// /// @param HTsl the list of all locations
// /// @param shelf_name a given location of a given shelf
// bool shelf_unique(ioopm_hash_table_t *HTsl, string shelf_name);

/// @brief checks if a string is a valid shelf location
/// @param shelf a given string representing the shelf
bool is_shelf(char *shelf);

/// @brief Checks if a item already exist
/// @param name the name of the item to be checked
bool item_unique(ioopm_hash_table_t *HTn, ioopm_item_t *item);

// /// @brief obtains ability for the user to make a item
// /// @return the newly created item
// ioopm_item_t *ioopm_input_item();

/// @brief Destroys a given item
/// @param item_Destr The item to be destroyed
void ioopm_destroy_item(ioopm_item_t *item_Destr);

/// @brief remove a specific item from warehouse
/// @param warehouse is the warehouse where the item is removed
/// @param key the name of the item to be removed
/// @return the item that is removed
/// @warning the returend item needs to be freed
bool ioopm_remove_item(ioopm_warehouse_t *warehouse, ioopm_item_t *item);

/// @brief Allows changing the name, description and price of a merch
/// @param warehouse where item is located and to be potentially edited
/// @param tmp the item to be edited
void edit_item(ioopm_warehouse_t *warehouse, ioopm_item_t *tmp);

/// @brief Creates a warehouse
/// @return a empty warehouse
/// @warning Needs to be destroyed
ioopm_warehouse_t ioopm_create_warehouse();

/// @brief destroys a given warehouse
/// @param warehouse the warehouse to be destroyed
void ioopm_warehouse_destroy(ioopm_warehouse_t *warehouse);

/// @brief Increases the stock of a merch by at least one
/// @param warehouse where the merch is located and to be potentially replenished
/// @param item the item to obtain stock location of
/// @param amount the amount of items to replenish to stock
bool replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item, size_t amount);

/// @brief list all items in the store (20 at a time) in alphabetical order
/// @param HTn are all the items in the warehouse
/// @param no_items the total number of current items in the warehouse
void list_db(ioopm_hash_table_t *HTn, size_t no_items);

/// @brief list all stock locations for a given item
/// @param item the item
void show_stock_db(ioopm_item_t item);

/// @brief asks a question where the expectation of the answer is a valid shelf location
/// @param question is the asked question
/// @return return the answered shelf location as a string
char *ioopm_ask_question_shelf(char *question);

/// @brief obtains ability to pick a specific item based on a list of all items
/// @param HTn are all the items in the warehouse
/// @return the choosen item of the list of items
ioopm_item_t *ioopm_choose_item_from_list(ioopm_hash_table_t *HTn);

/// @brief convert a hash table to an array
/// @param HTn is the hashtable to be converted
/// @return array containing the keys of the hash table
string *ioopm_item_array(ioopm_hash_table_t *HTn);

/// @brief Destroys a mechandice list
/// @param item The list to be destroyed
/// @param size The size of the list
void ioopm_item_list_destroy(string *item, size_t size);

/// @brief convert a hash table to an array
/// @param llsl is the linked list to be converted
/// @return array containing the converted linked list
string *ioopm_llsl_array(ioopm_list_t *llsl);