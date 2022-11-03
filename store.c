// #include "store.h"
#include "Back_end/db_back_end.h"
#include "db.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"
#include "Back_end/shopping_cart.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>


// använder
bool is_menu_char(char *c)
{
    if (strstr("LliTtsVRgkuncoA", c) == NULL || strlen(c) > 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
// använder
string print_menu()
{
    return "[L]ägga till en vara        \n"
           "[T]a bort en vara           \n"
           "[V]isa alla stocks          \n"
           "[R]edigera en vara          \n"
           "Lägg til[l] stock           \n"
           "Ån[g]ra senaste ändringen   \n"
           "Skappa [k]undvagn           \n"
           "Lägg till i k[u]ndvagnen    \n"
           "Ta bor[t] kundvagnen        \n"
           "Ta bort från kundvagne[n]   \n"
           "Che[c]kout                  \n"
           "T[o]tala kostnad            \n"
           "Lista alla [i]tems          \n"
           "[A]vsluta                   \n";
}

answer_t str_to_answer_t(string s)
{
    return (answer_t){.string_value = s};
}

// använder
char ask_question_menu() // TODO Free answer.
{
    answer_t answer = ask_question(print_menu(), is_menu_char, (convert_func)conv_str_answer);
    char answ = *answer.string_value;
    free(answer.string_value);
    return answ;
}

static void quit(ioopm_warehouse_t *warehouse, ioopm_hash_table_t *all_carts)
{
    ioopm_destroy_cart_list(all_carts);
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
            ioopm_list_items_in_cart(cart_choice); // TODO not working
        }

        char choice = ask_question_menu();
        if (choice == 'L')
        {
            ioopm_item_t *new_item = ioopm_input_item();
            if (ioopm_add_item(warehouse.HTn, new_item))
            {
                no_items++;
            }
        }
        else if (choice == 'T')
        {
            if (no_items != 0)
            {
                item = ioopm_choose_item_from_list(warehouse.HTn);
                if (cart_choice != NULL)
                {
                    if (ioopm_is_in_shopping_cart(cart_choice, item))
                    {
                        ioopm_remove_from_cart(cart_choice, item);
                    }
                }
                ioopm_remove_item(&warehouse, item->name);

                item = NULL;
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
                edit_item(&warehouse, item);
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
                int amount = ask_question_int("How many?");
                while (amount > (int)item->llsl->size || amount <= 0)
                {
                    amount = ask_question_int("Invalid input!");
                }
                ioopm_add_to_cart(cart_choice, item, amount);
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
                ioopm_destroy_cart(cart_choice);
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
                ioopm_remove_from_cart(cart_choice, item);
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
                // ioopm_hash_table_remove(all_carts, ioopm_int_to_elem(ioopm_cart_id(cart_choice)));
                // ioopm_destroy_cart(cart_choice);
                quit(&warehouse, all_carts);
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
                printf("%d cents\n", calculate_cost(cart_choice));
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
            quit(&warehouse, all_carts);
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
