#pragma once

#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct item item_t;

// item_t ioopm_make_item(char *name, char *descp, int price, char *shelf);

void ioopm_print_item(item_t *item);

// bool is_shelf(char *shelf);

char* ioopm_ask_question_shelf(char *question);

item_t ioopm_input_item(void);

char *magick(char *arr1[], char *arr2[], char *arr3[], int length);

void list_db(item_t *items, int no_items);

void edit_db(item_t *items, int no_items)

