
#include "utils.h"

struct item
{
    char *name;
    char *desc;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_item(item_t *item) {
    printf(" Name: %s \n Desc: %s \n Price: %d.%d SEK \n Shelf: %s \n", item->name, item->desc, (item->price) / 100, (item->price) % 100, item->shelf);
}

item_t make_item(char *n, char *d, int p, char *s) {
    item_t item = {.name = n, .desc = d, .price = p, .shelf = s};
    return item;
}

bool is_shelf(char *shelf) {
    
    if(isalpha(shelf[0]) && (string_length(shelf) > 1)) {
        for(int i = 1; i < string_length(shelf); i++) {
            if (!(isdigit(shelf[i]))) return false;
        }
        return true;
    }
    return false;
}

char *ask_question_shelf(char *question) {
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}

item_t input_item() {
    char *name = ask_question_string("What is the name of the item? ");
    char *desc = ask_question_string("Give a description of the item: ");
    int price = ask_question_int("What is the items price? ");
    char *shelf = ask_question_shelf("What shelf can you find the item on? ");
    return make_item(name, desc, price, shelf);
}

char *magick( char *arr1[], char *arr2[], char *arr3[], int num) {
    char buf[255] = {0};
    int rand1 = random() % num;
    int rand2 = random() % num;
    int rand3 = random() % num;
    printf("%d %d %d\n", rand1, rand2, rand3);
    strcat(buf, arr1[rand1]);
    strcat(buf, "-");
    strcat(buf, arr2[rand2]);
    strcat(buf, " ");
    strcat(buf, arr3[rand3]);
    return strdup(buf);
}

void list_db(item_t *items, int no_items) {
    for(int i = 1; i <= no_items; i++) {
        printf("%d. %s\n", i, items->name);
        ++items;
    }
}

void edit_db(item_t *items) {
    int num = ask_question_int("Which item would you like to edit? ");
    items += (num-1);
    print_item(items);
    *items = input_item();
}

bool is_menu_char(char *c) {
    if(strstr("LlTtRrGgHhAa", c) == NULL || strlen(c) > 1) {
        return false;
    }else {
        return true;
    }
}

char *print_menu() {
    return "[L]ägga till en vara        \n"
           "[T]a bort en vara           \n"
           "[R]edigera en vara          \n"
           "Ån[g]ra senaste ändringen   \n"
           "Lista [h]ela varukatalogen  \n"
           "[A]vsluta                   \n";
}

char string_to_char(char *s) {
    return *s;
}

char ask_question_menu() {
    answer_t answer = ask_question(print_menu() , is_menu_char, (convert_func) string_to_char);
    return toupper(answer.int_value);
}

void add_item_to_db(item_t *items, int *no_items) {
    if(*no_items < 16) {
        items += *no_items;
        *items = input_item();
        ++*no_items;
    }else {
        puts("Database already full!");
    }
}

void remove_item_from_db(item_t *items, int *no_items) {
    list_db(items, *no_items);
    int num = ask_question_int("Which item would you like to remove? ");
    items += (num-1);
    for(int i = num; i <= *no_items; i++) {
        *items = *(items+1);
        ++items;
    }
    --*no_items;
}

void event_loop(item_t *items, int *no_items) {
    int loop = 1;
    while(loop) {
        char choice = ask_question_menu();
        if(choice == 'L') {
            add_item_to_db(items, no_items);
        }else if(choice == 'T') {
            remove_item_from_db(items, no_items);
        }else if(choice == 'R') {
            edit_db(items);
        }else if(choice == 'G') {
            puts("Not yet implemented");
        }else if(choice == 'H') {
            list_db(items, *no_items);
        }else if(choice == 'A') {
            loop = 0;
        }

    }
}

int main(int argc, char *argv[])
{
    srandom(time(NULL));
    char *array1[] = {"Laser",        "Polka",    "Extra"}; // TODO: Lägg till!
    char *array2[] = {"förnicklad",   "smakande", "ordinär"}; // TODO: Lägg till!
    char *array3[] = {"skruvdragare", "kola",     "uppgift"}; // TODO: Lägg till!

    if (argc < 2)
    {
        printf("Usage: %s number\n", argv[0]);
    }
    else
    {
        item_t db[16]; // Array med plats för 16 varor
        int db_siz = 0; // Antalet varor i arrayen just nu

        int items = atoi(argv[1]); // Antalet varor som skall skapas

        if (items > 0 && items <= 16)
        {
        for (int i = 0; i < items; ++i)
        {
            // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
            item_t item = input_item();
            db[db_siz] = item;
            ++db_siz;
        }
        }
        else
        {
        puts("Sorry, must have [1-16] items in database.");
        return 1; // Avslutar programmet!
        }
        puts("Hej");
        for (int i = db_siz; i < 16; ++i)
        {
            
            char *name = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
            char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
            int price = random() % 200000;
            char shelf[] = { random() % ('Z'-'A') + 'A',
                            random() % 10 + '0',
                            random() % 10 + '0',
                            '\0' };
            item_t item = make_item(name, desc, price, shelf);

            db[db_siz] = item;
            ++db_siz;
        }


        event_loop(db, &db_siz);
        
    }
    
    return 0;
}