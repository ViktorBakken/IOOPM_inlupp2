// #include "store.h"
#include "Back_end/back_end.h"
#include "db.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#define ERR_non_unique "Error item is non unique!"

ioopm_item_t add_merchandise(ioopm_hash_table_t *HTn)
{
    ioopm_item_t new_item = ioopm_input_item();

    if (merchandice_unique(HTn, new_item.name))
    {
        ioopm_add_merchandise_backend(HTn, &new_item);
        return new_item;
    }
    else
    {
        perror(ERR_non_unique);
        assert(false);
    }

    return empty_item_t;
}

ioopm_item_t edit_merchandise(ioopm_warehouse_t warehouse, ioopm_item_t *tmp)
{
    char *answername = ask_question_string("Do you wish to change the name?: ");
    if (strcmp(answername, "yes"))
    {
        char *answername2 = ask_question_string("What name would you like to name it?: ");
        if (merchandice_unique(warehouse.HTn, answername2))
        {
            tmp->name = answername2;
        }
        else
        {
            puts("That name of a item already exists!");
        }
    }
    else
    {
        char *answerdesc = ask_question_string("Do you wish to change the description?: ");
        if (strcmp(answerdesc, "yes"))
        {
            char *answerdesc2 = ask_question_string("What shall the new description be?: ");
            tmp->desc = answerdesc2;
        }
        else
        {
            char *answerprice = ask_question_string("Would you like to change the price?: ");
            if (strcmp(answerprice, "yes"))
            {
                size_t newprice = ask_question_int("What would you like to put the new price on?: ");
                tmp->price = newprice;
            }
        }
    }
    puts("Should not be able to get here!");
    assert(false);
}

void replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item) // TODO
{
    (void) item;

    list_db(warehouse->HTn, ioopm_hash_table_size(warehouse->HTn));
    ioopm_item_t choosen_item = ioopm_choose_item_from_list(warehouse->HTn);
    
    int amount = ask_question_int("How much of this item?: ");
    string shelf_name;
    for(int i = 0; i < amount; i++){
        shelf_name = ioopm_random_shelf();

        if (shelf_unique(warehouse->HTsl, shelf_name)) /*|| uniqe_shelf_location)*/
        {
            // add to the specefic stock
            ioopm_linked_list_prepend(choosen_item.llsl, ioopm_str_to_elem(shelf_name));
            
        }
    }
}

void quit() // TODO
{
}

void event_loop(int no_items, ioopm_warehouse_t warehouse)
{
    ioopm_item_t item;
    while (true)
    {
        char choice = ask_question_menu();
        if (choice == 'L')
        {
            add_merchandise(warehouse.HTn);
            no_items++;

        }
        else if (choice == 'T')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            ioopm_remove_merchandise(&warehouse, item.name);
            no_items--;
        }
        else if (choice == 'R')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            edit_merchandise(warehouse, &item);
        }
        else if (choice == 'V')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            show_stock_db(item);
        }
        else if (choice == 's')
        {
            // list_db(warehouse->HTn, no_items);
        }
        else if (choice == 'l')
        {
            // replenish_stock();
        }
        else if (choice == 'A')
        {
            // quit();
        }
        else if (choice == 'k')
        {
            // create_cart();
        }
        else if (choice == 'u')
        {
            // add_to_cart();
        }
        // else if (choice == 't')
        // {
        //     // remove_cart();
        // }
        else if (choice == 'n')
        {
            // remove_from_cart();
        }
        else if (choice == 'c')
        {
            // checkout();
        }
        else if (choice == 'o')
        {
            // calculate_cost();
        }
        else if (choice == 't')
        {
            list_db(warehouse.HTn, no_items);
        }
    }
}

int main(void)
{
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    int no_items = ioopm_hash_table_size(warehouse.HTn);

    // event_loop(no_items, warehouse,)
    event_loop(no_items, warehouse);

    return 0;
}
