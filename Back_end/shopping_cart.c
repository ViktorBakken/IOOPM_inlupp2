#include "shopping_cart.h"

#define string char *


struct cart{
    int id;
    ioopm_hash_table_t *cart;
};

typedef struct cart_item cart_item_t;
struct cart_item{
    int amount;
    ioopm_item_t item;
};

ioopm_shopping_cart_t create_cart(int id)
{
    ioopm_hash_table_t *hash_table_cart = create_cart_backend();
    return (ioopm_shopping_cart_t) {.id = id, .cart = hash_table_cart};
}

// void remove_cart(ioopm_shopping_cart_t *cart)
// {
//     remove_cart_backend(cart); //problem! remove_cart_backend() takes a hash_table_t!!!
// }

void remove_cart(ioopm_shopping_cart_t *cart)
{
    remove_cart_backend(cart->cart);
}

void add_to_cart(ioopm_warehouse_t warehouse, ioopm_shopping_cart_t *cart, int index, int amount) // TODO
{
    (void) cart;
    (void) index;
    (void) amount;
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
    (void) item;
    

    // ioopm_add_to_cart(cart,item.name, amount);

}

void remove_from_cart(ioopm_shopping_cart_t *cart) //TODO ta en extra titt, osäker vad den gör!
{
    remove_cart_backend(cart->cart);
}

int calculate_cost(ioopm_shopping_cart_t *cart, ioopm_warehouse_t warehouse)
{
    int total;
    ioopm_list_t *keys = ioopm_hash_table_keys(cart->cart);
    for(int i = 0; i < (int) ioopm_linked_list_size(keys); i++){
        elem_t index = ioopm_linked_list_get(keys, i);
        void *item_ptr = ioopm_hash_table_lookup(warehouse.HTn, index).value.p;
        ioopm_item_t *item_choosen = item_ptr;

        void *amount_ptr = ioopm_hash_table_lookup(cart->cart, index).value.p;
        cart_item_t *choosen_item_in_cart = amount_ptr;

        total = item_choosen->price * choosen_item_in_cart->amount;
    }
    return total;
}


void checkout(ioopm_shopping_cart_t *cart, ioopm_warehouse_t *warehouse) // TODO add for special case when we buy out item
{
    
    ioopm_list_t *keys = ioopm_hash_table_keys(cart->cart);
    for(int i = 0; i < (int) ioopm_linked_list_size(keys); i++){
        elem_t index = ioopm_linked_list_get(keys, i);
        void *item_ptr = ioopm_hash_table_lookup(warehouse->HTn, index).value.p;
        ioopm_item_t *item_choosen = item_ptr;
        
        void *amount_ptr = ioopm_hash_table_lookup(cart->cart, index).value.p;
        cart_item_t *cart_item = amount_ptr;        
        item_choosen->llsl->size =- cart_item->amount; //Du glömde lägga till =


    }

    
    remove_cart(cart);
}


