#include "db.h"

static bool shelf_unique(ioopm_hash_table_t *HTsl, string shelf_name)
{
    return !ioopm_hash_table_has_key(HTsl, ioopm_str_to_elem(shelf_name));
}

static bool is_shelf(string shelf)
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

static void destroy_shelf_listfunc(size_t unused_index, elem_t *value, void *unused_extra)
{
    (void)unused_index;
    (void)unused_extra;
    free(value->s);
}

bool item_unique(ioopm_hash_table_t *HTn, ioopm_item_t *item)
{
    return !ioopm_hash_table_has_key(HTn, ioopm_str_to_elem(item->name));
}

static string ioopm_random_shelf()
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

ioopm_item_t *ioopm_input_item()
{
    string name = ask_question_string("What is the name of the item? ");
    string desc = ask_question_string("Give a description of the item: ");
    int price = ask_question_int("What is the items price? ");

    return make_item_backend(name, desc, price);
}

void ioopm_destroy_item(ioopm_item_t *item_Destr)
{
    free(item_Destr->name);
    free(item_Destr->desc);
    ioopm_linked_list_apply_to_all(item_Destr->llsl, destroy_shelf_listfunc, NULL);
    ioopm_linked_list_destroy(item_Destr->llsl);
    free(item_Destr);
}

void destroy_item_hashfunc(elem_t unused_key, elem_t *value, void *unused_extra)
{
    (void)unused_extra;
    (void)unused_key;
    ioopm_item_t *item_ptr = value->p;
    ioopm_destroy_item(item_ptr);
}

bool ioopm_remove_item(ioopm_warehouse_t *warehouse, string key) // TODO FIX
{
    if (ioopm_hash_table_lookup(warehouse->HTn, ioopm_str_to_elem(key)).success)
    {
        ioopm_item_t *item_ptr = ioopm_hash_table_remove(warehouse->HTn, ioopm_str_to_elem(key)).value.p;
        ioopm_list_t *item_llsl = item_ptr->llsl;
        for (size_t i = 0; i < item_llsl->size ; i++)
        {
            string current_shelf = ioopm_linked_list_get(item_llsl, i).s;
            ioopm_hash_table_remove(warehouse->HTsl, ioopm_str_to_elem(current_shelf));
        }
        
        ioopm_destroy_item(item_ptr);
        return true;
    }
    return false;
}

void edit_item(ioopm_warehouse_t *warehouse, ioopm_item_t *item)
{
    char *answername = ask_question_string("Do you wish to change the name?: ");
    if (strstr("yes", answername))
    {
        char *answername2 = ask_question_string("What name would you like to name it?: ");
        if (item_unique(warehouse->HTn, item))
        {
            ioopm_hash_table_remove(warehouse->HTn, ioopm_str_to_elem(item->name));
            strcpy(item->name, answername2);
            ioopm_hash_table_insert(warehouse->HTn, ioopm_str_to_elem(item->name), ioopm_ptr_to_elem(item));

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

            strcpy(item->desc, newdesc2);
            free(newdesc2);
        }
        else
        {
            char *answerprice = ask_question_string("Would you like to change the price?: ");
            if (strstr("yes", answerprice))
            {
                size_t newprice = ask_question_int("What would you like to put the new price on?: ");
                item->price = newprice;
            }
            free(answerprice);
        }
        free(answerdesc);
    }
    free(answername);
}

ioopm_warehouse_t ioopm_create_warehouse()
{
    ioopm_hash_table_t *HTn = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    ioopm_hash_table_t *HTsl = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    return (ioopm_warehouse_t){.HTn = HTn, .HTsl = HTsl};
}

void ioopm_warehouse_destroy(ioopm_warehouse_t *warehouse) // TODO free strings, DONE?(ger valgrind error?)
{
    ioopm_hash_table_apply_to_all(warehouse->HTn, destroy_item_hashfunc, NULL);
    ioopm_hash_table_destroy(warehouse->HTn);
    ioopm_hash_table_destroy(warehouse->HTsl);
}

void replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item, size_t amount)
{
    
    string shelf_name;
    bool found_non_unique_shelf;
    for (size_t i = 0; i < amount; i++)
    {
        found_non_unique_shelf = true;
        while (found_non_unique_shelf)
        {
            string item = ioopm_random_shelf();
            if (shelf_unique(warehouse->HTsl, item))
            {
                shelf_name = item;
                found_non_unique_shelf = false;
            }
            else
            {
                free(item);
            }
        }

        ioopm_linked_list_prepend(item->llsl, ioopm_str_to_elem(shelf_name));
        ioopm_hash_table_insert(warehouse->HTsl, ioopm_str_to_elem(shelf_name), ioopm_ptr_to_elem(item));
    }
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
                (void)answer;
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
    for (int i = 0; i < (int)size; i++)
    {
        printf("%d. %s\n", i + 1, locationarray[i]);
    }
    free(locationarray);
}

string ask_question_shelf(string question)
{
    return ask_question(question, is_shelf, conv_str_answer).string_value; // TODO strdup returns a string not a answear_t!
}

ioopm_item_t *ioopm_choose_item_from_list(ioopm_hash_table_t *HTn)
{
    size_t size = ioopm_hash_table_size(HTn);

    list_db(HTn, (int)size);

    int index = -1;
    while (0 > index || index > (int)size)
    {
        index = ask_question_int("Whiche item?"); // TODO add test for valid/negative inputs, etc
        index--;
    }
    return ioopm_choose_item_from_list_backend(HTn, index);
}
string *ioopm_llsl_array(ioopm_list_t *llsl) // NEED TO FREE keys
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(llsl);
    char **keys = calloc(llsl->size, sizeof(char *));

    for (size_t i = 0; i < llsl->size; i++)
    {
        keys[i] = ioopm_iterator_current(iter).s;
        if (i < llsl->size - 1)
        {
            ioopm_iterator_next(iter);
        }
    }
    ioopm_iterator_destroy(iter);
    return keys;
}

void ioopm_item_list_destroy(string *item, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
        free(item[i]);
    }
    free(item);
}

string *ioopm_item_array(ioopm_hash_table_t *HTn) // TODO NEED TO FREE keys
{
    ioopm_list_t *list = ioopm_hash_table_keys(HTn);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    string *keys = calloc(list->size, sizeof(string));

    for (size_t i = 0; i < list->size; i++)
    {
        keys[i] = strdup(ioopm_iterator_current(iter).s);
        if (i < list->size - 1)
        {
            ioopm_iterator_next(iter);
        }
    }
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iter);
    return keys;
}