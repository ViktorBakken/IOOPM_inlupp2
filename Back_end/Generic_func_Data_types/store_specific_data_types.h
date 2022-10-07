#pragma once/// @brief Insert at the end of a linked list in O(1) time
#include "common.h"

#define string char *

typedef struct item ioopm_item_t;
struct item {
    string name;
    string desc;
    size_t price; 
    ioopm_list_t *llsl;  //linked list stock location! 
};

typedef struct warehouse ioopm_warehouse_t;
struct warehouse {
    ioopm_hash_table_t *HTn;    //hash table names! 
    ioopm_hash_table_t *HTsl;   //hash table stock location!
};