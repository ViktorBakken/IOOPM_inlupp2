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

int ioopm_cart_id(ioopm_shopping_cart_t *cart){
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

cart_item_t *create_cart_item(int amount, ioopm_item_t *item)
{
    cart_item_t *cart_item = calloc(1, sizeof(cart_item_t));
    cart_item->amount = amount;
    cart_item->item = item;
    return cart_item;
}

static void print_item(ioopm_item_t *item)
{
    printf("%s: %d amount in cart\n", item->name, (int)item->price);
}

void ioopm_list_items_in_cart(ioopm_shopping_cart_t *cart)
{
    if (cart && ioopm_hash_table_size(cart->cart) != 0)
    {

        ioopm_list_t *value_list = ioopm_hash_table_values(cart->cart); // TODO INNEHÅLLER SKRäP
        ioopm_list_iterator_t *value_iterator = ioopm_list_iterator(value_list);
        ioopm_item_t *current_item = ioopm_iterator_current(value_iterator).p;

        do
        {
            print_item(current_item);
            ioopm_iterator_next(value_iterator);
        } while (ioopm_iterator_has_next(value_iterator));

        ioopm_linked_list_destroy(value_list);
        ioopm_iterator_destroy(value_iterator);
    }
}

void remove_cart(ioopm_shopping_cart_t *cart)
{
    if (cart != NULL)
    {
        remove_cart_backend(cart->cart);
        free(cart);
    }
    else
    {
        perror(ERR_NULL_cart);
        assert(false);
    }
}

void ioopm_destroy_cart(elem_t unused_key, elem_t *value, void *unused_extra)
{
    (void)unused_extra;
    (void)unused_key;

    ioopm_shopping_cart_t *cart = value->p;
    destroy_cart_backend(cart->cart);
    free(cart);
}

void add_to_cart(ioopm_shopping_cart_t *cart, size_t amount, ioopm_item_t *item)
{
    if (cart->cart != NULL && item->llsl->size >= amount)
    {
        cart_item_t *cart_item = create_cart_item(amount, item);

        ioopm_hash_table_insert(cart->cart, ioopm_str_to_elem(item->name), ioopm_ptr_to_elem(cart_item)); // TODO leaves rubish
    }
    else
    {
        puts("No stock avalible!");
    }
}

void remove_from_cart(ioopm_shopping_cart_t *cart, ioopm_item_t *item_rem)
{
    int size = ioopm_ht_size(cart->cart);
    if (cart != NULL && size != 0)
    {
        ioopm_remove_from_cart(cart->cart, item_rem->name);
    }
    else
    {
        perror("No items to remove!");
    }
}

int calculate_cost(ioopm_shopping_cart_t *cart)
{
    int total;
    // ioopm_list_t *keys = ioopm_hash_table_keys(cart->cart);
    // for (int i = 0; i < (int)ioopm_linked_list_size(keys); i++)
    // {
    //     elem_t index = ioopm_linked_list_get(keys, i);
    //     void *item_ptr = ioopm_hash_table_lookup(warehouse.HTn, index).value.p;
    //     ioopm_item_t *item_choosen = item_ptr;

    //     void *amount_ptr = ioopm_hash_table_lookup(cart->cart, index).value.p;
    //     cart_item_t *choosen_item_in_cart = amount_ptr;

    //     total = item_choosen->price * choosen_item_in_cart->amount;
    // }

    ioopm_list_t *values = ioopm_hash_table_values(cart->cart);
    for (int i = 0; i < (int)ioopm_linked_list_size(values); i++)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(values, i).p;

        total += cart_item->amount * cart_item->item->price;
    }
    ioopm_linked_list_destroy(values); // TODO might not remove cart_item
    return total;
}

bool checkout(ioopm_shopping_cart_t *cart, ioopm_hash_table_t *HTsl)
{
    // ioopm_list_t *keys = ioopm_hash_table_keys(cart->cart);
    // for (int i = 0; i < (int)ioopm_linked_list_size(keys); i++)
    // {
    //     elem_t index = ioopm_linked_list_get(keys, i);
    //     void *item_ptr = ioopm_hash_table_lookup(warehouse->HTn, index).value.p;
    //     ioopm_item_t *item_choosen = item_ptr;

    //     void *amount_ptr = ioopm_hash_table_lookup(cart->cart, index).value.p;
    //     cart_item_t *cart_item = amount_ptr;
    //     if (item_choosen->llsl->size != 0)
    //     {
    //         item_choosen->llsl->size -= cart_item->amount;
    //     }
    //     else
    //     {
    //         assert(false);
    //     }
    // }
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
            remove_cart(cart);
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
