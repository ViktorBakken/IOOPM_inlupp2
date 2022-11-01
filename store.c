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

bool add_merchandise(ioopm_hash_table_t *HTn)
{
    ioopm_item_t *new_item = ioopm_input_item();

    if (merchandice_unique(HTn, new_item->name))
    {
        ioopm_add_merchandise_backend(HTn, new_item);
        return true;
    }
    perror(ERR_non_unique);
    return false;
}

void edit_merchandise(ioopm_warehouse_t *warehouse, ioopm_item_t *tmp)
{
    char *answername = ask_question_string("Do you wish to change the name?: ");
    if (strstr("yes", answername))
    {
        char *answername2 = ask_question_string("What name would you like to name it?: ");
        if (merchandice_unique(warehouse->HTn, answername2))
        {
            ioopm_hash_table_remove(warehouse->HTn, ioopm_str_to_elem(tmp->name));
            strcpy(tmp->name, answername2);
            ioopm_hash_table_insert(warehouse->HTn, ioopm_str_to_elem(tmp->name), ioopm_ptr_to_elem(tmp));
            
            free(answername2);
        }
        else
        {
            puts("That name of a item already exists!");
        }
    }
    else
    {
        char *answerdesc = ask_question_string("Do you wish to change the description?: ");
        if (strstr("yes", answerdesc))
        {
            char *newdesc2 = ask_question_string("What shall the new description be?: ");

            strcpy(tmp->desc, newdesc2);
            free(newdesc2);
        }
        else
        {
            char *answerprice = ask_question_string("Would you like to change the price?: ");
            if (strstr("yes", answerprice))
            {
                size_t newprice = ask_question_int("What would you like to put the new price on?: ");
                tmp->price = newprice;
            }
            free(answerprice);
        }
        free(answerdesc);
    }
    free(answername);
}

void replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item)
{
    int amount = ask_question_int("How much of this item?: ");
    string shelf_name;
    bool found_non_unique_shelf;
    for (size_t i = 0; i < (size_t)amount; i++)
    {
        found_non_unique_shelf = true;
        while (found_non_unique_shelf)
        {
            string tmp = ioopm_random_shelf();
            if (shelf_unique(warehouse->HTsl, tmp))
            {
                shelf_name = tmp;
                found_non_unique_shelf = false;
            }
            else
            {
                free(tmp);
            }
        }

        ioopm_linked_list_prepend(item->llsl, ioopm_str_to_elem(shelf_name));
        ioopm_hash_table_insert(warehouse->HTsl, ioopm_str_to_elem(shelf_name), ioopm_ptr_to_elem(item));
    }
}

static void quit(ioopm_warehouse_t *warehouse)
{
    ioopm_warehouse_destroy(warehouse);
    exit(0);
}

void event_loop(int no_items, ioopm_warehouse_t warehouse)
{
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    ioopm_item_t *item;
    ioopm_shopping_cart_t *cart_choice = NULL;

    bool avaliable_shopping_cart = false;

    while (true)
    {
        if (avaliable_shopping_cart)
        {
            puts("\nshoppingcart:");
            // ioopm_list_items_in_cart(cart_choice); // TODO not working
        }

        char choice = ask_question_menu();
        if (choice == 'L')
        {
            if (add_merchandise(warehouse.HTn))
            {
                no_items++;
            }
        }
        else if (choice == 'T')
        {
            if (no_items != 0)
            {
                item = ioopm_choose_item_from_list(warehouse.HTn);
                ioopm_remove_merchandise(&warehouse, item->name);
                no_items--;
            }
            else
            {
                puts("No items to remove\n");
            }
        }
        else if (choice == 'V')
        {
            if (no_items != 0)
            {
                item = ioopm_choose_item_from_list(warehouse.HTn);
                show_stock_db(*item);
            }
            else
            {
                puts("No items!\n");
            }
        }
        else if (choice == 'R')
        {
            if (no_items != 0)
            {
                item = ioopm_choose_item_from_list(warehouse.HTn);
                edit_merchandise(&warehouse, item);
            }
            else
            {
                puts("No items to edit\n");
            }
        }
        else if (choice == 'l')
        {
            if (no_items != 0)
            {
                item = ioopm_choose_item_from_list(warehouse.HTn);
                replenish_stock(&warehouse, item);
            }
            else
            {
                puts("No items to add!\n");
            }
        }
        else if (choice == 'g')
        {
            puts("Not implemented!");
        }
        else if (choice == 'k')
        {
            if (cart_choice == NULL)
            {
                int id = ask_question_int("What shall the id of your cart be");
                create_cart(all_carts, id);
                cart_choice = choose_cart(all_carts, id);
                avaliable_shopping_cart = true;
            }
            else
            {
                puts("Already a pending cart\n");
            }
        }
        else if (choice == 'u')
        {
            if (cart_choice != NULL && no_items != 0)
            { 
                item = ioopm_choose_item_from_list(warehouse.HTn);
                int amount = ask_question_int("How many...?: ");
                add_to_cart(cart_choice, amount, item);
            }
            else 
            {
                puts("No cart exists or no items in the cart\n");
            }
        }
        else if (choice == 't')
        {
            if (cart_choice != NULL)
            {
                ioopm_hash_table_remove(all_carts, ioopm_int_to_elem(ioopm_cart_id(cart_choice)));
                remove_cart(cart_choice);
                cart_choice = NULL;

                avaliable_shopping_cart = false;
            }
            else
            {
                puts("No carts to remove\n");
            }
        }
        else if (choice == 'n')
        {
            if (no_items != 0 && cart_choice != NULL)
            {
                item = ioopm_choose_item_from_list(warehouse.HTn);
                remove_from_cart(cart_choice, item);
                item = NULL;
            }
            else
            {
                puts("Not yet duable!(no cart or no items in the cart)\n");
            }
        }
        else if (choice == 'c')
        {
            if (cart_choice != NULL)
            {
                checkout(cart_choice, warehouse.HTsl);
                ioopm_hash_table_remove(all_carts, ioopm_int_to_elem(ioopm_cart_id(cart_choice)));
                remove_cart(cart_choice);
                cart_choice = NULL;
                avaliable_shopping_cart = false;
            }
            else
            {
                puts("No carts made!\n");
            }
        }
        else if (choice == 'o')
        {
            if (/*TODO no_items != 0 && ger inga items 0?*/ cart_choice != NULL)
            {
                calculate_cost(cart_choice);
            }
            else
            {
                puts("No carts made!\n");
            }
        }
        else if (choice == 'i')
        {
            if (no_items != 0)
            {
                list_db(warehouse.HTn, no_items);
            }
            else
            {
                puts("No items to list!\n");
            }
        }
        else if (choice == 'A')
        {
            ioopm_destroy_cart_list(all_carts);
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
