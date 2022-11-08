#include "store.h"

static void ioopm_edit_item(ioopm_warehouse_t *warehouse, ioopm_item_t *item)
{

    char *answername = ask_question_string("Do you wish to change the name? (write yes): ");
    char to_lower;
    for (size_t i = 0; i < strlen(answername); i++)
    {
        to_lower = tolower(answername[i]);
        answername[i] = to_lower;
    }

    if (strstr("yes", answername))
    {
        char *answername2 = ask_question_string("What name would you like to name it?: ");
        if (ioopm_item_name_unique(warehouse->HTn, answername2))
        {
            ioopm_hash_table_remove(warehouse->HTn, ioopm_str_to_elem(item->name));
            char *tmp_name = item->name;
            // void *tmp = tmp_name;
            item->name = answername2;
            free(tmp_name);
            ioopm_hash_table_insert(warehouse->HTn, ioopm_str_to_elem(item->name), ioopm_ptr_to_elem(item));
        }
        else
        {
            puts("That name of a item already exists!");
        }
        // free(answername2);
    }

    char *answerdesc = ask_question_string("Do you wish to change the description? (write yes): ");
    for (size_t i = 0; i < strlen(answerdesc); i++)
    {
        to_lower = tolower(answerdesc[i]);
        answerdesc[i] = to_lower;
    }

    if (strstr("yes", answerdesc))
    {
        char *newdesc2 = ask_question_string("What shall the new description be?: ");
        char *tmp_desc = item->desc;
        item->desc = newdesc2;
        free(tmp_desc);
    }

    char *answerprice = ask_question_string("Would you like to change the price? (write yes): ");
    for (size_t i = 0; i < strlen(answerprice); i++)
    {
        to_lower = tolower(answerprice[i]);
        answerprice[i] = to_lower;
    }

    if (strstr("yes", answerprice))
    {
        size_t newprice = ask_question_int("What would you like the new price to be?: ");
        item->price = newprice;
    }

    free(answerprice);
    free(answerdesc);
    free(answername);
}

static ioopm_item_t *ioopm_input_item()
{
    string name = ask_question_string("What is the name of the item? ");
    string desc = ask_question_string("Give a description of the item: ");
    int price = ask_question_int("What is the items price?: ");
    while (price < 0)
    {
        price = ask_question_int("Negative price input!");
    }
    
    return ioopm_make_item_backend(name, desc, price);
}

static bool is_menu_char(char *c)
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
static string print_menu()
{
    return "[L]ägga till en vara        (Create item)\n"
           "[T]a bort en vara           (Remove item)\n"
           "[V]isa alla stocks          (Show item stock)\n"
           "[R]edigera en vara          (Edit an item)\n"
           "Lägg til[l] stock           (Increase stock)\n"
           "Skappa [k]undvagn           (Create cart)\n"
           "Lägg till i k[u]ndvagnen    (Add to cart)\n"
           "Ta bor[t] kundvagnen        (Remove cart)\n"
           "Ta bort från kundvagne[n]   (Remove from cart)\n"
           "Che[c]kout                  (Checkout)\n"
           "T[o]tala kostnad            (Total cost of cart)\n"
           "Lista alla [i]tems          (List all items)\n"
           "[A]vsluta                   (Quit)\n";
}

// använder
static char ask_question_menu() // Need to free answer after use
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

static void event_loop(int no_items, ioopm_warehouse_t warehouse)
{
    ioopm_hash_table_t *all_carts = ioopm_create_cart_list();
    ioopm_item_t *item;
    ioopm_shopping_cart_t *cart_choice = NULL;

    bool avaliable_shopping_cart = false;
    int amount = -1;

    while (true)
    {
        if (avaliable_shopping_cart)
        {
            puts("\nshoppingcart:");
            ioopm_list_items_in_cart(cart_choice);
        }

        char choice = ask_question_menu();
        if (choice == 'L')
        {
            ioopm_item_t *new_item = ioopm_input_item();
            if (ioopm_add_item(warehouse.HTn, new_item))
            {
                no_items++;
            }
            else
            {
                free(new_item);
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
                ioopm_remove_item(&warehouse, item);

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
                ioopm_show_stock_items(*item);
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
                ioopm_edit_item(&warehouse, item);
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
                amount = ask_question_int("How much?");
                while (amount < 0)
                {
                    amount = ask_question_int("Invalid amount!!");
                }

                ioopm_replenish_stock(&warehouse, item, (size_t)amount);
            }
            else
            {
                puts("No items to add!\n");
            }
        }
        else if (choice == 'k')
        {
            if (cart_choice == NULL)
            {
                int id = ask_question_int("What shall the id of your cart be");
                ioopm_create_cart_in_cart_list(all_carts, id);
                cart_choice = ioopm_choose_cart(all_carts, id);
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
                if (item->llsl->size > 0)
                {
                    int amount = ask_question_int("How many?");
                    while (amount > (int)item->llsl->size || amount <= 0)
                    {
                        amount = ask_question_int("Not enough in stock or invalid input!");
                    }
                    ioopm_add_to_cart(cart_choice, item, amount);
                }
                else
                {
                    puts("Item not in stock!");
                }
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
                ioopm_checkout(cart_choice, warehouse.HTsl);
                ioopm_hash_table_remove(all_carts, ioopm_int_to_elem(ioopm_cart_id(cart_choice)));
                ioopm_destroy_cart(cart_choice);
                // quit(&warehouse, all_carts);
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
            if (no_items != 0 && cart_choice != NULL)
            {
                printf("%d cents\n", ioopm_calculate_cost(cart_choice));
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
                ioopm_list_item(warehouse.HTn, no_items);
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
