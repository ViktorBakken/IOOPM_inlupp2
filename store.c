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

    if (merchandice_unique(warehouse.HTn, name))
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

void list_merchandise() // TODO
{
    // list_db();
}

ioopm_item_t edit_merchandise(ioopm_warehouse_t warehouse, ioopm_item_t *tmp) // TODO
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
    while (true)
    {
        char choice = ask_question_menu();
        if (choice == 'L')
        {
            // add_merchandise(items, no_items, "", 0);
        }
        else if (choice == 'T')
        {
            ioopm_item_t tmp = ioopm_choose_item_from_list(warehouse.HTn);
            ioopm_remove_merchandise(&warehouse, tmp.name);
        }
        else if (choice == 'R')
        {
            ioopm_item_t tmp = ioopm_choose_item_from_list(warehouse.HTn);
            edit_merchandise(warehouse, &tmp);
        }
        else if (choice == 'V')
        {
            // show_stock(warehouse, item);
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
        else if (choice == 't')
        {
            // remove_cart();
        }
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

int main(int argc, char const *argv[])
{
    // ioopm_item_t item = ioopm_input_item();
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    int no_items = ioopm_hash_table_size(warehouse.HTn);

    event_loop(no_items, warehouse);

    return 0;
}
