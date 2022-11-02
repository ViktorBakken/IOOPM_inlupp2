#include "db.h"

#define ERR_non_unique "Error item is non unique!"


bool ioopm_add_item(ioopm_hash_table_t *HTn)
{
    ioopm_item_t *new_item = ioopm_input_item();

    if (item_unique(HTn, new_item->name))
    {
        ioopm_add_item_backend(HTn, new_item);
        return true;
    }
    perror(ERR_non_unique);
    return false;
}

void edit_item(ioopm_warehouse_t *warehouse, ioopm_item_t *tmp)
{
    char *answername = ask_question_string("Do you wish to change the name?: ");
    if (strstr("yes", answername))
    {
        char *answername2 = ask_question_string("What name would you like to name it?: ");
        if (item_unique(warehouse->HTn, answername2))
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


bool is_shelf(string shelf)
{
    if (isalpha(shelf[0]) && (string_length(shelf) > 1))
    {
        for (int i = 1; i < string_length(shelf); i++)
        {
            if (!(isdigit(shelf[i])))
                return false;
        }
        return true;
    }
    return false;
}

// ioopm_item_t ioopm_item_destroy(ioopm_item_t *item_rem){

// }
string ioopm_random_shelf()
{ // TODO fix magic numbers
    int constant = 10;

    char buf[3];
    int rand1 = random() % constant;
    int rand2 = random() % constant;
    int rand3 = random() % 26;

    char rand1_char = (char)(rand1 + 48); // ascii(48) = '0'
    char rand2_char = (char)(rand2 + 48); // ascii(48) = '0'
    char letter = (char)(rand3 + 65);     // ascii(65) = 'A'

    buf[0] = letter;
    buf[1] = rand1_char;
    buf[2] = rand2_char;

    return strdup(buf);
}

string ask_question_shelf(string question)
{
    return ask_question(question, is_shelf, conv_str_answer).string_value; // TODO strdup returns a string not a answear_t!
}

ioopm_item_t *ioopm_input_item()
{
    string name = ask_question_string("What is the name of the item? ");
    string desc = ask_question_string("Give a description of the item: ");
    int price = ask_question_int("What is the items price? ");

    return make_item_backend(name, desc, price);
}

ioopm_item_t *ioopm_choose_item_from_list(ioopm_hash_table_t *HTn)
{

    list_db(HTn, (int)ioopm_ht_size(HTn));
    int index = -1;
    while ((0 > index || index > (int)ioopm_ht_size(HTn)))
    {
        index = ask_question_int("Whiche item?"); // TODO add test for valid/negative inputs, etc
        index--;
    }
    return ioopm_choose_item_from_list_backend(HTn, index);
}

void list_db(ioopm_hash_table_t *HTn, size_t no_items)
{
    string *item = ioopm_item_array(HTn);

    qsort(item, no_items, sizeof(string), cmpstringp); // taken from freq-count.c

    for (size_t i = 0; i < no_items; i++)
    {
        printf("%d. %s\n", (int)i + 1, item[i]);
        if (i % 20 == 0 && i != 0)
        {
            string answer = ask_question_string("List more items?\n");
            char to_lower;
            for (size_t i = 0; i < strlen(answer); i++)
            {
                to_lower = tolower(answer[i]);
                answer[i] = to_lower;
            }

            while (strcmp(answer, "no") != 0 && strcmp(answer, "yes") != 0) // TODO we assume that input is in small letters! not accepted: write Yes| yEs| YES
            {
                puts("Invalid input!/n");
                free(answer);
                string answer = ask_question_string("List more items?\n");
                (void) answer;
            }

            if (strcmp(answer, "no") == 0)
            {
                free(answer);
                break;
            }
            free(answer);
        }
    }
ioopm_item_list_destroy(item, no_items);
}


void show_stock_db(ioopm_item_t item)
{
    ioopm_list_t *locations = item.llsl;
    size_t size = item.llsl->size;
    string *locationarray = ioopm_llsl_array(locations);
    qsort(locationarray, ioopm_linked_list_size(locations), sizeof(char *), cmpstringp);

    puts("Item is availabel at locations:");
    for (int i = 0; i < (int) size; i++)
    {
        printf("%d. %s\n", i + 1, locationarray[i]);
    }
    free(locationarray);
}

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