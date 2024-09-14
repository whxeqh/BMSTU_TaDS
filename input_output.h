#ifndef INPUT_OUTPUT_H__
#define INPUT_OUTPUT_H__

#include <stdio.h>

#ifdef DEBUG
void print_start_info(FILE *file);
#endif

int input_nums(char *str1, char *str2, FILE *file);


#endif
