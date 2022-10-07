#include "utils.h"

void clear_input_buffer() {
    int c;
    do
        {
          c = getchar();
        }
      while (c != '\n' && c != EOF);
}

bool not_empty(char *str) {
    return strlen(str) > 0;
}

void print(char *str) {
    for(int i = 0; i < string_length(str); i++) {
        putchar(str[i]);
    }
}

void println(char *str) {
    print(str);
    putchar('\n');
}

bool is_number(char *str) {

    if(str[0] == '-' || isdigit(str[0])) {
        for(int i = 1; i < strlen(str); i++) {
            if (isdigit(str[i]) == false) return false;
        }
        return true;
    }
    return false;
}

int read_string(char *buf, int buf_siz) {
    int count = 0;
    int c;

    while(count < buf_siz) {
        c = getchar();
        if(c == '\n') {
            break;
        }
        buf[count] = c;
        count++;
    }
    buf[count] = '\0';
    if(count == buf_siz) clear_input_buffer();
    return count;
}

answer_t ask_question(char *question, check_func check, convert_func convert) {
    int buf_siz = 255;
    char buf[buf_siz];
    do {
        println(question);
        read_string(buf, buf_siz);
        if(check(buf)) {
            break;
        }
    } while(true);

    return convert(buf);
}

int ask_question_int(char *question) {
    answer_t answer = ask_question(question, is_number, (convert_func) atoi);
    return answer.int_value; // svaret som ett heltal
}

char *ask_question_string(char *question) {
  return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

int string_length(char *str) {
    int i = 0;
    for(;str[i] != '\0';i++){

    }
return i;
}