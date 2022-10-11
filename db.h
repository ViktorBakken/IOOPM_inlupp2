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


ioopm_item_t ioopm_make_item(char *name, char *descp, int price, char *shelf);

void ioopm_print_item(ioopm_item_t *item);

bool is_shelf(char *shelf);

char* ioopm_ask_question_shelf(char *question);

ioopm_item_t ioopm_input_item(void);

char *magick(char *arr1[], char *arr2[], char *arr3[], int length);

void list_db(ioopm_hash_table_t *HTn, int no_items);

void edit_db(ioopm_item_t *items, int no_items);

char ask_question_menu();


