#include "db.h"

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

string ioopm_random_shelf()
{ // TODO fix magic numbers
    int constant = 10;


    char buf[3];
    int rand1 = random() % constant;
    int rand2 = random() % constant;
    int rand3 = random() % 26;

    char rand1_char = (char)(rand1 + 48);  //ascii(48) = '0'
    char rand2_char = (char)(rand2 + 48);   //ascii(48) = '0'
    char letter = (char)(rand3 + 65);   //ascii(65) = 'A'

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
    int index = ask_question_int("Whiche item?"); // TODO test for valid/negative inputs, etc
    index--;
    return ioopm_choose_item_from_list_backend(HTn, index);
}

void list_db(ioopm_hash_table_t *HTn, int no_items)
{
    string answer;

    string *merchandise = ioopm_merchandice_array(HTn);

    qsort(merchandise, no_items, sizeof(string), cmpstringp); // taken from freq-count.c

    for (int i = 0; i < no_items; i++)
    {
        printf("%d. %s\n", i + 1, merchandise[i]);
        if (i % 20 == 0 && i != 0)
        {
            answer = ask_question_string("List more items?\n");

            while (strcmp(answer, "no") != 0 && strcmp(answer, "yes") != 0) // TODO we assume that input is in small letters! not accepted: write Yes| yEs| YES
            {
                puts("Invalid input!/n");
                answer = ask_question_string("List more items?\n");
            }

            if (strcmp(answer, "no") == 0)
            {
                break;
            }
        }
    }
    for (size_t i = 0; i < no_items; i++)
    {
       free(merchandise[i]);
    }
    
    free(merchandise);
}
/*void edit_db(ioopm_item_t *items, int no_items)
{
    // int num = ask_question_int("Which item would you like to edit? ");
    // items += (num - 1);
    // print_item(items);
    // *items = input_item();
}
*/

void show_stock_db(ioopm_item_t item)
{
    ioopm_list_t *locations = item.llsl;
    string *locationarray = ioopm_llsl_array(locations);
    qsort(locationarray, ioopm_linked_list_size(locations), sizeof(char *), cmpstringp);

    puts("Item is availabel at locations:");
    for (int i = 0; i < (int)item.llsl->size; i++)
    {
        printf("%d. %s\n", i + 1, locationarray[i]);
    }
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
char ask_question_menu()
{
    answer_t answer = ask_question(print_menu(), is_menu_char, (convert_func)str_to_answer_t);
    return *answer.string_value;
}