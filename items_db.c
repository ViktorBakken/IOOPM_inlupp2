#include "items_db.h"

static bool shelf_unique(ioopm_hash_table_t *HTsl, string shelf_name)
{
    return !ioopm_hash_table_has_key(HTsl, ioopm_str_to_elem(shelf_name));
}

bool ioopm_is_shelf(string shelf)
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

bool ioopm_item_name_unique(ioopm_hash_table_t *HTn, string item_name)
{
    return !ioopm_hash_table_has_key(HTn, ioopm_str_to_elem(item_name));
}

static string random_shelf()
{
    int int_modulo_num = 10;
    int int_modulo_char = 26;
    int ascii_addition_num = 48; // ascii(48) = '0'
    int ascii_addition_chr = 65; // ascii(65) = 'A'

    char buf[3];
    int rand1 = random() % int_modulo_num;
    int rand2 = random() % int_modulo_num;
    int rand3 = random() % int_modulo_char;

    char rand1_char = (char)(rand1 + ascii_addition_num);
    char rand2_char = (char)(rand2 + ascii_addition_num);
    char letter = (char)(rand3 + ascii_addition_chr);

    buf[0] = letter;
    buf[1] = rand1_char;
    buf[2] = rand2_char;

    return strdup(buf);
}

void ioopm_destroy_item(ioopm_item_t *item_Destr)
{
    free(item_Destr->name);
    free(item_Destr->desc);
    ioopm_linked_list_apply_to_all(item_Destr->llsl, destroy_shelf_listfunc, NULL);
    ioopm_linked_list_destroy(item_Destr->llsl);
    free(item_Destr);
}

static void destroy_item_hashfunc(elem_t unused_key, elem_t *value, void *unused_extra)
{
    (void)unused_extra;
    (void)unused_key;
    ioopm_item_t *item_ptr = value->p;
    ioopm_destroy_item(item_ptr);
}

bool ioopm_remove_item(ioopm_warehouse_t *warehouse, ioopm_item_t *item)
{
    if (ioopm_hash_table_has_key(warehouse->HTn, ioopm_str_to_elem(item->name)))
    {
        ioopm_item_t *item_ptr = ioopm_hash_table_remove(warehouse->HTn, ioopm_str_to_elem(item->name)).value.p;
        ioopm_list_t *item_llsl = item_ptr->llsl;
        for (size_t i = 0; i < item_llsl->size; i++)
        {
            string current_shelf = ioopm_linked_list_get(item_llsl, i).s;
            ioopm_hash_table_remove(warehouse->HTsl, ioopm_str_to_elem(current_shelf));
        }

        ioopm_destroy_item(item_ptr);
        return true;
    }
    return false;
}

ioopm_warehouse_t ioopm_create_warehouse()
{
    ioopm_hash_table_t *HTn = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    ioopm_hash_table_t *HTsl = ioopm_hash_table_create(ioopm_elem_str_eq, ioopm_elem_item_eq, ioopm_string_hash);
    return (ioopm_warehouse_t){.HTn = HTn, .HTsl = HTsl};
}

void ioopm_warehouse_destroy(ioopm_warehouse_t *warehouse)
{
    ioopm_hash_table_apply_to_all(warehouse->HTn, destroy_item_hashfunc, NULL);
    ioopm_hash_table_destroy(warehouse->HTn);
    ioopm_hash_table_destroy(warehouse->HTsl);
}

bool ioopm_replenish_stock(ioopm_warehouse_t *warehouse, ioopm_item_t *item, size_t amount)
{

    string shelf_name;
    bool found_non_unique_shelf;
    if (ioopm_hash_table_has_key(warehouse->HTn, ioopm_str_to_elem(item->name)))
    {
        if (amount > 0)
        {
            for (size_t i = 0; i < amount; i++)
            {
                found_non_unique_shelf = true;
                while (found_non_unique_shelf)
                {
                    string item = random_shelf();
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
            return true;
        }
    }
    return false;
}

void ioopm_list_item(ioopm_hash_table_t *HTn, size_t no_items)
{
    char to_lower;
    ioopm_item_t *current_item;

    string *item = ioopm_item_array(HTn);
    qsort(item, no_items, sizeof(string), cmpstringp); // taken from freq-count.c

    for (size_t i = 0; i < no_items; i++)
    {
        current_item = ioopm_hash_table_lookup(HTn, ioopm_str_to_elem(item[i])).value.p;

        printf("%d. Name of item: %s  (%d nr in stock)\n''%s''\n\n", (int)i + 1, current_item->name, (int) current_item->llsl->size, current_item->desc);
        if (i % 20 == 0 && i != 0)
        {
            string answer = ask_question_string("List more items?\n");

            for (size_t i = 0; i < strlen(answer); i++)
            {
                to_lower = tolower(answer[i]);
                answer[i] = to_lower;
            }

            while (strcmp(answer, "no") != 0 && strcmp(answer, "yes") != 0)
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

void ioopm_show_stock_items(ioopm_item_t item)
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

ioopm_item_t *ioopm_choose_item_from_list(ioopm_hash_table_t *HTn)
{
    size_t size = ioopm_hash_table_size(HTn);

    ioopm_list_item(HTn, (int)size);

    int index = -1;
    while (0 > index || index > (int)size)
    {
        index = ask_question_int("Whiche item?");
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

string *ioopm_item_array(ioopm_hash_table_t *HTn) // free keys after use
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