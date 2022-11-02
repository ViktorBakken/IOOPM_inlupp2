#include "shopping_cart.h"
#include <assert.h>
#define string char *

#define ERR_NULL_cart "Not an existing cart..."
#define ERR_NULL_item "Not an existing item..."

struct cart
{ // TODO ty då vi nu använder hashtable för att förvara alla olika shopping all_carts, behöver vi kanske inte spara alla id i structen då id används som key.
    int id;
    ioopm_hash_table_t *cart;
};

int ioopm_cart_id(ioopm_shopping_cart_t *cart)
{
    return cart->id;
}

typedef struct cart_item cart_item_t;
struct cart_item
{
    int amount;
    ioopm_item_t *item;
};

bool ioopm_elem_cart_item_eq(elem_t elem1, elem_t elem2)
{
    cart_item_t *item1 = elem1.p;
    cart_item_t *item2 = elem2.p;
    return (0 == strcmp(item1->item->name, item2->item->name));
}

void create_cart(ioopm_hash_table_t *all_carts, int id)
{
    if (all_carts != NULL && cart_unique(all_carts, id))
    {
        ioopm_shopping_cart_t *new_cart = calloc(1, sizeof(ioopm_shopping_cart_t));
        ioopm_hash_table_t *hash_table_cart = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_cart_item_eq, ioopm_string_hash);

        new_cart->id = id;
        new_cart->cart = hash_table_cart;

        ioopm_hash_table_insert(all_carts, ioopm_int_to_elem(id), ioopm_ptr_to_elem(new_cart));
    }
    else
    {
        perror(ERR_NULL_cart);
        assert(false);
    }
}

static cart_item_t *create_cart_item(size_t amount, ioopm_item_t *item)
{
    cart_item_t *cart_item = calloc(1, sizeof(cart_item_t)); // must remove
    cart_item->amount = amount;
    cart_item->item = item;
    return cart_item;
}

void ioopm_add_to_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item)
{
    if (cart->cart != NULL)
    {
        if (!ioopm_is_in_shopping_cart(cart, item))
        {
            int amount = ask_question_int("How many?");
            while (amount > (int)item->llsl->size || amount <= 0)
            {
                amount = ask_question_int("Invalid input!");
            }

            cart_item_t *cart_item = create_cart_item((size_t)amount, item);

            ioopm_hash_table_insert(cart->cart, ioopm_str_to_elem(item->name), ioopm_ptr_to_elem(cart_item));
        }
        else{
            puts("No stock avalible!");
        }
    }
    else
    {
        puts("No stock avalible!");
    }
}

bool ioopm_is_in_shopping_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item)
{
    return ioopm_hash_table_has_key(cart->cart, ioopm_str_to_elem(item->name));
}

static void print_item(ioopm_item_t *item)
{
    printf("%s: %d cents/item\n", item->name, (int)item->price);
}

void ioopm_list_items_in_cart(ioopm_shopping_cart_t *cart)
{
    if (cart && ioopm_hash_table_size(cart->cart) != 0)
    {

        ioopm_list_t *value_list = ioopm_hash_table_values(cart->cart);
        ioopm_list_iterator_t *value_iterator = ioopm_list_iterator(value_list);
        cart_item_t *current_cart_item = ioopm_iterator_current(value_iterator).p;

        do
        {
            printf("%d amount of ", current_cart_item->amount);
            print_item(current_cart_item->item);
            ioopm_iterator_next(value_iterator);
        } while (ioopm_iterator_has_next(value_iterator));
        printf("\n");
        ioopm_linked_list_destroy(value_list);
        ioopm_iterator_destroy(value_iterator);
    }
}

void remove_cart(ioopm_shopping_cart_t *cart)
{
    if (cart != NULL)
    {
        ioopm_hash_table_destroy(cart->cart);
        free(cart);
    }
    else
    {
        perror(ERR_NULL_cart);
    }
}

void ioopm_destroy_cart(elem_t unused_key, elem_t *value, void *unused_extra)
{
    (void)unused_extra;
    (void)unused_key;

    ioopm_shopping_cart_t *cart = value->p;

    ioopm_list_t *cart_item_list = ioopm_hash_table_values(cart->cart);
    ioopm_list_iterator_t *cart_item_iterator = ioopm_list_iterator(cart_item_list);
    cart_item_t *current_cart_item = ioopm_iterator_current(cart_item_iterator).p;

    do
    {
        free(current_cart_item);
        ioopm_iterator_next(cart_item_iterator);
    } while (ioopm_iterator_has_next(cart_item_iterator));

    ioopm_linked_list_destroy(cart_item_list);
    ioopm_iterator_destroy(cart_item_iterator);
    destroy_cart_backend(cart->cart);
    free(cart);
}

void ioopm_remove_from_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item_rem)
{
    int size = ioopm_ht_size(cart->cart);
    if (cart != NULL && size != 0)
    {
        ioopm_hash_table_remove(cart->cart, ioopm_str_to_elem(item_rem->name));
    }
    else
    {
        perror("No items to remove!");
    }
}

int calculate_cost(ioopm_shopping_cart_t *cart)
{
    if (cart) // Handeling NULL cart case
    {
        int total = 0;

        ioopm_list_t *cart_item_list = ioopm_hash_table_values(cart->cart);
        for (int i = 0; i < (int)ioopm_linked_list_size(cart_item_list); i++)
        {
            cart_item_t *cart_item = ioopm_linked_list_get(cart_item_list, i).p;

            total += cart_item->amount * cart_item->item->price;
        }
        ioopm_linked_list_destroy(cart_item_list);
        return total;
    }
    return -1;
}

bool checkout(ioopm_shopping_cart_t *cart, ioopm_hash_table_t *HTsl)
{
    if (cart) // Handeling cart is NULL
    {
        printf("Your total is ");
        if (ioopm_hash_table_size(cart->cart) > 0) // Handeling cart is empty
        {
            printf("%d kr\nThanks for shopping with us!\n", calculate_cost(cart));

            ioopm_list_t *values = ioopm_hash_table_values(cart->cart);
            ioopm_list_iterator_t *cart_iter = ioopm_list_iterator(values);
            size_t amount;
            size_t items_in_stock;
            for (int i = 0; i < (int)ioopm_linked_list_size(values); i++)
            {
                cart_item_t *cart_item = cart_iter->current->value.p;

                amount = cart_item->amount;
                items_in_stock = cart_item->item->llsl->size;

                if (amount <= items_in_stock) // failsafe checker
                {
                    for (size_t i = 0; i < amount; i++)
                    {
                        string shelf_rem = ioopm_linked_list_remove(cart_item->item->llsl, 0).s;
                        ioopm_hash_table_remove(HTsl, ioopm_str_to_elem(shelf_rem));
                        free(shelf_rem);
                    }
                }
                else
                {
                    perror("There is not enough items in stock");
                    return false;
                }

                ioopm_iterator_next(cart_iter);
            }
            ioopm_iterator_destroy(cart_iter); // TODO might not remove cart_item
            ioopm_linked_list_destroy(values); // TODO might not remove cart_item
            // remove_cart(cart);
        }
        else
        {
            printf("%d kr\nThanks for shopping with us!\n", 0);
        }
        return true;
    }
    perror("Non-existing shopping cart!");
    return false;
}
