#pragma once
#include "common.h"

/**
 * @file store_specific_data_types.h
 * @author Loran Daqouri och Viktor Bakken
 * @date 32 Nov 2022
 * @brief Common datatypes for the store program.
 */

#define string char *
#define empty_item_t (ioopm_item_t){.desc="",.llsl=NULL, .name=""}
bool ioopm_elem_item_eq(elem_t elem1, elem_t elem2);

int cmpstringp(const void *p1, const void *p2);

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