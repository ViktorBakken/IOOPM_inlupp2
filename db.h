#pragma once
#include "Back_end/Generic_func_Data_types/utils.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"
#include "Back_end/back_end.h"

#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 


/// @brief Adds a new item to the warehouse with a name, description and price
/// @param HTn the current item's in the warehouse
/// @return true if the item is unique(and adds it), other false(not add it).
bool ioopm_add_item(ioopm_hash_table_t *HTn);

/// @brief Allows changing the name, description and price of a merch
/// @param warehouse where item is located and to be potentially edited
/// @param tmp the item to be edited
void edit_item(ioopm_warehouse_t *warehouse, ioopm_item_t *tmp);

/// @brief Increases the stock of a merch by at least one
/// @param warehouse where the merch is located and to be potentially replenished
/// @param item the item to obtain stock location of
void replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item);

/// @brief checks if a string is a valid shelf location
/// @param shelf a given string representing the shelf
bool is_shelf(char *shelf);

/// @brief Creates a random shelf
/// @return a string in the form of ex A14 
/// @warning need to free the return value!
string ioopm_random_shelf(); 

/// @brief checks if a shelf's location is unique
/// @param HTsl the list of all locations
/// @param shelf_name a given location of a given shelf
bool shelf_unique(ioopm_hash_table_t *HTsl, string shelf_name);

/// @brief asks a question where the expectation of the answer is a valid shelf location
/// @param question is the asked question
/// @return return the answered shelf location as a string
char* ioopm_ask_question_shelf(char *question);

/// @brief obtains ability to pick a specific item based on a list of all items
/// @param HTn are all the items in the warehouse
/// @return the choosen item of the list of items
ioopm_item_t *ioopm_choose_item_from_list(ioopm_hash_table_t *HTn);

/// @brief obtains ability for the user to make a item
/// @return the newly created item 
ioopm_item_t *ioopm_input_item();

/// @brief list all items in the store (20 at a time) in alphabetical order
/// @param HTn are all the items in the warehouse
/// @param no_items the total number of current items in the warehouse
void list_db(ioopm_hash_table_t *HTn, size_t no_items);

/// @brief list all stock locations for a given item
/// @param item the item
void show_stock_db(ioopm_item_t item);

/// @brief prints the menu with option to choose action based on a letter
/// @return the choosen letter to produce the respective action
char ask_question_menu();


