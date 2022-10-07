#include "store.h"
#include "Back_end/back_end.h"
#include "db.h"
#include "Back_end/Generic_func_Data_types/store_specific_data_types.h"


#define ERR_non_unique "Error item is non unique!"

void add_merchandise(ioopm_warehouse_t *warehouse, string name, string description, size_t price) //TODO
    {
        if (merchandice_unique(name))
        {
            // TODO     
        }
        else
        {
            perror(ERR_non_unique);
        }
    }

    void remove_merchandise() // TODO
    {
    }

    void list_merchandise() // TODO
    {
    }

    void edit_merchandise() // TODO
    {
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

    ask_question;

    int main(int argc, char const *argv[])
    {
        
        // event_loop();
        return 0;
    }
