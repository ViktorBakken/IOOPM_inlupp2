#include "shopping_cart.h"
#include "back_end.h"
#include "Generic_func_Data_types/store_specific_data_types.h"
#include "../db.h"


struct cart{
    int id;
    ioopm_hash_table_t *cart;
};

typedef struct cart_item cart_item_t;
struct cart_item{
    int amount;
    ioopm_item_t item;
};

void create_cart()
{
    ioopm_shopping_cart_t *cart = create_cart_backend();
}

void remove_cart(ioopm_shopping_cart_t *cart)
{
    remove_cart_backend(cart);
}

void add_to_cart(ioopm_warehouse_t warehouse, ioopm_shopping_cart_t *cart, int index, int amount) // TODO
{

    //Fråga vilket item som ska läggas till samt mängden av det itemet
    //kopiera över det itemet i cart_item samt mängden av det itemet
    // lägg till det cart_item i shoppingcarten
    //bli glad över en fungerande funktion :)


    list_db(warehouse.HTn, ioopm_ht_size(warehouse.HTn)); 
   /* int index = ask_question_int("Choose a item based on index: ");
    ioopm_list_t *keys = ioopm_hash_table_keys(warehouse.HTn);
    elem_t choice = ioopm_linked_list_get(keys, index);
    ioopm_option_t option = ioopm_hash_table_lookup(warehouse.HTn, choice);
    ioopm_item_t *tmp = option.value.p;
    ioopm_list_t *stocktmp = tmp->llsl;
*/
    ioopm_item_t item = ioopm_choose_item_from_list(warehouse.HTn);

    

    // ioopm_add_to_cart(cart,item.name, amount);

}

void remove_from_cart(ioopm_shopping_cart_t *cart)
{
    remove_cart_backend(cart);
}

int calculate_cost(ioopm_shopping_cart_t *cart, ioopm_warehouse_t warehouse)
{
    int total;
    ioopm_list_t *keys = ioopm_hash_table_keys(cart->cart);
    for(int i = 0; i < ioopm_linked_list_size(keys); i++){
        elem_t index = ioopm_linked_list_get(keys, i);
        void *item_ptr = ioopm_hash_table_lookup(warehouse.HTn, index).value.p;
        ioopm_item_t *item_choosen = item_ptr;

        void *amount_ptr = ioopm_hash_table_lookup(cart, index).value.p;
        cart_item_t *cart_item = amount_ptr;

        total = item_choosen->price * cart_item->amount;
    }
    return total;
}


void checkout(ioopm_shopping_cart_t *cart) // TODO
{

}


