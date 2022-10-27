// #include "store.h"
#include "Back_end/back_end.h"
#include "db.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"
#include "Back_end/shopping_cart.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#define ERR_non_unique "Error item is non unique!"
#define ID 1334

void add_merchandise(ioopm_hash_table_t *HTn)
{
    ioopm_item_t *new_item = ioopm_input_item();

    if (merchandice_unique(HTn, new_item->name))
    {
        ioopm_add_merchandise_backend(HTn, new_item);
    }
    else
    {
        perror(ERR_non_unique);
        assert(false);
    }
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

void replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item) // TODO DONE?
{
    int amount = ask_question_int("How much of this item?: ");
    string shelf_name;
    for (int i = 0; i < amount; i++)
    {
        do
        {
            shelf_name = ioopm_random_shelf();

        } while (shelf_unique(warehouse->HTsl, shelf_name));

        ioopm_linked_list_prepend(item->llsl, ioopm_str_to_elem(shelf_name));
    }
}

static void quit(ioopm_warehouse_t *warehouse)
{
    ioopm_warehouse_destroy(warehouse);
    exit(0);
}

void event_loop(int no_items, ioopm_warehouse_t warehouse)
{
    ioopm_hash_table_t *carts = ioopm_create_cart_list();
    ioopm_item_t *item;
    ioopm_shopping_cart_t *cart_choice = NULL;

    int id = ID;
    bool avaliable_shopping_cart = false;

    while (true)
    {
        if (avaliable_shopping_cart) // TODO add function to show all the items in the shoppingcart
        {
            puts("\nshoppingcart\n");
        }

        char choice = ask_question_menu();
        if (choice == 'L')
        {
            add_merchandise(warehouse.HTn);
            no_items++;
        }
        else if (choice == 'T')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            ioopm_remove_merchandise(&warehouse, item->name);
            no_items--;
        }
        else if (choice == 'V')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            show_stock_db(*item);
        }
        else if (choice == 'R')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            edit_merchandise(warehouse, item);
        }
        else if (choice == 'l')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            replenish_stock(&warehouse, item);
        }
        else if (choice == 'g')
        {
            // TODO implementera ångra ändring
        }
        else if (choice == 'k')
        {
            create_cart(carts, id); // TODO Create random int (or something)
            avaliable_shopping_cart = true;
        }
        else if (choice == 'u')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            int amount = ask_question_int("How many...?: ");
            add_to_cart(carts, amount, item); // TODO
        }
        else if (choice == 't') // TODO add case for non-existing shopping cart, DONE?
        {
            remove_cart(cart_choice);
            cart_choice = NULL;

            avaliable_shopping_cart = false;
        }
        else if (choice == 'n')
        {
            item = ioopm_choose_item_from_list(warehouse.HTn);
            remove_from_cart(cart_choice, item);
            item = NULL;
        }
        else if (choice == 'c')
        {
            checkout(cart_choice, &warehouse);
            remove_cart(cart_choice);
            cart_choice = NULL;
        }
        else if (choice == 'o')
        {
            calculate_cost(cart_choice, warehouse);
        }
        else if (choice == 'i')
        {
            list_db(warehouse.HTn, no_items);
        }
        else if (choice == 'A')
        {
            ioopm_destroy_cart_list(carts);
            quit(&warehouse);
        }
    }
}

int main(void)
{
    srandom(time(NULL));
    ioopm_warehouse_t warehouse = ioopm_create_warehouse();
    int no_items = ioopm_hash_table_size(warehouse.HTn);

    event_loop(no_items, warehouse);

    return 0;
}
