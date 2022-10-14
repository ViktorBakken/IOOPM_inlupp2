#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

extern char *strdup(const char *);

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef int(*int_fold_func)(int, int);
typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);

int read_string(char *buf, int buf_siz);
bool is_number(char *str);
int ask_question_int(char *question);
char *ask_question_string(char *question);
int string_length(char *str);
void print(char *str);
void println(char *str);
bool not_empty(char *str);
answer_t ask_question(char *question, check_func check, convert_func convert);
#endif 