// #include "store.h"
#include "Back_end/back_end.h"
#include "db.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>


#define ERR_non_unique "Error item is non unique!"

ioopm_item_t add_merchandise(ioopm_warehouse_t warehouse, string name, string description, size_t price) // TODO
{
    
    if (merchandice_unique(name))
    {
        ioopm_item_t tmp = ioopm_input_item();
        return tmp;
    }
    else
    {
        perror(ERR_non_unique);
    }
    
    return empty_item_t;
}


ioopm_item_t remove_merchandise(ioopm_warehouse_t ,size_t index) // TODO
{
    // if (merchandice_unique(name))
    // {
    //     ioopm_elem_str_eq()
    //     ioopm_item_t tmp = warehouse.Htn;
    //     return tmp;
    // }
    // else
    // {
    //     perror(ERR_non_unique);
    // }
    return empty_item_t;
}


void list_merchandise() // TODO
{
    //list_db();
}

void edit_merchandise() // TODO
{




}

void show_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item) // TODO
{
    string name = item->name;
    string stock_and_number;
    
}

void replenish_stock(string location, size_t amount) // TODO
{
}

void create_cart() // TODO
{
}

void remove_cart() // TODO
{
}

void add_to_cart() // TODO
{
}

void remove_from_cart() // TODO
{
}

void calculate_cost() // TODO
{
}

void checkout() // TODO
{
}

void quit() // TODO
{
}


void event_loop(int no_items, ioopm_warehouse_t warehouse) // TODO update for new ioopm_item_t
{
    int loop = 1;
    while (loop)
    {
        char choice = ask_question_menu();
        if (choice == 'L')
        {
            //add_merchandise(items, no_items, "", 0);
        }
        else if (choice == 'T')
        {
            //remove_merchandise();
        }
        else if (choice == 'R')
        {
            //edit_merchandise();
        }
        else if (choice == 'V')
        {
            // show_stock(warehouse, item);
        }
        else if (choice == 's')
        {
            //list_db(warehouse->HTn, no_items);
        }
        else if (choice == 'l')
        {
            //replenish_stock();
        }
        else if (choice == 'A')
        {
            //quit();
        }
        else if (choice == 'k')
        {
            //create_cart();
        }
        else if (choice == 'u')
        {
            //add_to_cart();
        }
        else if (choice == 't')
        {
            //remove_cart();
        }
        else if (choice == 'n')
        {
            //remove_from_cart();
        }
        else if (choice == 'c')
        {
            //checkout();
        }
        else if (choice == 'o')
        {
            //calculate_cost();
        }
        else if (choice == 't')
        {
            //list_merchandise();
        }
    }
}

int main(int argc, char const *argv[])
{
    ioopm_item_t item = ioopm_input_item();
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    int no_items = ioopm_hash_table_size(warehouse.HTn);

    event_loop(no_items, warehouse);

    
    return 0;
}
