#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef int(*int_fold_func)(int, int);
typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);
/// @brief remove a specific item from warehouse
/// @param warehouse is the warehouse where the item is removed
/// @param key the name of the item to be removed
/// @return the item that is removed
/// @warning the returend item needs to be freed

/// @brief obtains the given string letter by letter
/// @param buf is the buffer containing the given letters in a string
/// @param buf_siz is the size of the buffer(the given string)
/// @return the length of the given string
int read_string(char *buf, int buf_siz);

/// @brief checks if a given string is a number(including negativ numbers)
/// @param str is the given string to be checked
/// @return a true if it is a number, otherwise a false
bool is_number(char *str);

/// @brief asks a question where the expectation of the answer is a number
/// @param question is the asked question
/// @return the answer to the question(only if answer is a number)
int ask_question_int(char *question);

/// @brief asks a question where the expectation of the answer is a string
/// @param question is the question to be asked
/// @return the answer to the question(only if answer is a char *, i.e string)
char *ask_question_string(char *question);

/// @brief obtains the length of a string
/// @param str a string
/// @return the length of a given string
int string_length(char *str);

/// @brief prints a given string to the terminal
/// @param str a string to be printed
void print(char *str);

/// @brief prints a given string and a newline
/// @param str a string to be printed along a newline afterwards
void println(char *str);

/// @brief checks so that a string contains characters(is not empty)
/// @param str the string to be checked on
/// @return boolean value
bool not_empty(char *str);

/// @brief basis function for the ask functions
/// @param question is the question to be asked
/// @param check a function to apply on the answer with object of checking
/// @param convert a function to apply on the answer with object of converting
/// @return a answer_t representing all types of answers(int, float, string)
answer_t ask_question(char *question, check_func check, convert_func convert);

/// @brief convert a string to a int
/// @param str is the string to be converted
/// @return a answer_t representing a int in this case
answer_t conv_int_answer(char *str);

/// @brief copies a given string
/// @param str is the string to copy
/// @return a answer_t representing a string in this case
answer_t conv_str_answer(char *str);
#endif 