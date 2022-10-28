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
    int constant = 9;
    string rand1_str = " ";
    string rand2_str = " ";


    char buf[255] = {0};
    int rand1 = random() % constant;
    int rand2 = random() % constant;
    int rand3 = random() % 26;
    char letter = (char)(rand3 + 65);

    sprintf(rand1_str, "%d", rand1);
    sprintf(rand2_str, "%d", rand2);

    strcat(buf, &letter);
    strcat(buf, rand1_str);
    strcat(buf, rand2_str);
    
    return strdup(buf);
}

string ask_question_shelf(string question)
{
    return ask_question(question, is_shelf, conv_str_answer).string_value; //TODO strdup returns a string not a answear_t!
}

ioopm_item_t ioopm_input_item()
{
    string name = ask_question_string("What is the name of the item? ");
    string desc = ask_question_string("Give a description of the item: ");
    int price = ask_question_int("What is the items price? ");

    return make_item_backend(name, desc, price);
}

ioopm_item_t ioopm_choose_item_from_list(ioopm_hash_table_t *HTn)
{
    int index = ask_question_int("Whiche item?"); // TODO test for negative inputs, etc

    return ioopm_choose_item_from_list_backend(HTn, index);
}


// string magick(string arr1[], string arr2[], string arr3[], int num)
// {
//     char buf[255] = {0};
//     int rand1 = random() % num;
//     int rand2 = random() % num;
//     int rand3 = random() % num;
//     printf("%d %d %d\n", rand1, rand2, rand3);
//     strcat(buf, arr1[rand1]);
//     strcat(buf, "-");
//     strcat(buf, arr2[rand2]);
//     strcat(buf, " ");
//     strcat(buf, arr3[rand3]);
//     return strdup(buf);
// }

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
            
            while ( strcmp(answer, "no") != 0 && strcmp(answer, "yes") != 0) //TODO we assume that input is in small letters! not accepted: write Yes| yEs| YES
            {
                puts("Invalid input!/n");
                answer = ask_question_string("List more items?\n");
            }

            if (strcmp(answer, "no") == 0 )
            {
                break;
            }
        }
    }
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

    for (int i = 0; i < (int) ioopm_linked_list_size(locations); i++)
    {
        printf("%d. %s: %d", i + 1, locationarray[i], (int) item.llsl->size);
    }
}

// använder
bool is_menu_char(char *c)
{
    if (strstr("LlTtsVRgkuncoA", c) == NULL || strlen(c) > 1)
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
           "Li[s]ta alla items          \n"
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
           "Lis[t]a alla items          \n"
           "[A]vsluta                   \n";
}

answer_t str_to_answer_t(string s)
{
    return (answer_t) {.string_value = s};
}

// använder
char ask_question_menu()
{
    answer_t answer = ask_question(print_menu(), is_menu_char, (convert_func)str_to_answer_t);
    return *answer.string_value;
}

// int main(int argc, char *argv[])
// {
//     srandom(time(NULL));
//     char *array1[] = {"Laser", "Polka", "Extra"};           // TODO: Lägg till!
//     char *array2[] = {"förnicklad", "smakande", "ordinär"}; // TODO: Lägg till!
//     char *array3[] = {"skruvdragare", "kola", "uppgift"};   // TODO: Lägg till!

//     if (argc < 2)
//     {
//         printf("Usage: %s number\n", argv[0]);
//     }
//     else
//     {
//         ioopm_item_t db[16]; // Array med plats för 16 varor
//         int db_siz = 0;      // Antalet varor i arrayen just nu

//         int items = atoi(argv[1]); // Antalet varor som skall skapas

//         if (items > 0 && items <= 16)
//         {
//             for (int i = 0; i < items; ++i)
//             {
//                 // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
//                 ioopm_item_t item = input_item();
//                 db[db_siz] = item;
//                 ++db_siz;
//             }
//         }
//         else
//         {
//             puts("Sorry, must have [1-16] items in database.");
//             return 1; // Avslutar programmet!
//         }
//         puts("Hej");
//         for (int i = db_siz; i < 16; ++i)
//         {

//             char *name = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
//             char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
//             int price = random() % 200000;
//             char shelf[] = {random() % ('Z' - 'A') + 'A',
//                             random() % 10 + '0',
//                             random() % 10 + '0',
//                             '\0'};
//             ioopm_item_t item = make_item(name, desc, price, shelf);

//             db[db_siz] = item;
//             ++db_siz;
//         }

//         // event_loop(db, &db_siz);
//     }

//     return 0;
// }