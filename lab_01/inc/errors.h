#ifndef ERRORS_H__
#define ERRORS_H__

#define OK                0
#define ERR_IO            2
#define ERR_INPUT_INT     3
#define ERR_INPUT_REAL    4
#define ERR_RANGE         5

#include <stdio.h>

void print_error(int rc, FILE *file);

#endif
