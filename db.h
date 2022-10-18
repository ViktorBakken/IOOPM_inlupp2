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


ioopm_item_t ioopm_make_item(char *name, char *descp, int price, char *shelf);

bool is_shelf(char *shelf);

string ioopm_random_shelf();

bool shelf_unique(ioopm_hash_table_t *HTsl, string shelf_name);

char* ioopm_ask_question_shelf(char *question);

ioopm_item_t ioopm_choose_item_from_list(ioopm_hash_table_t *HTn);

ioopm_item_t ioopm_input_item();

void list_db(ioopm_hash_table_t *HTn, int no_items);

void show_stock_db(ioopm_item_t item);

char ask_question_menu();


