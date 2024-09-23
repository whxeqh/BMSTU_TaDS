#ifndef __COUNTRIES_ARRAY_H__
#define __COUNTRIES_ARRAY_H__

#include <stdio.h>
#include <stddef.h>

#include "structures.h"

#define MAX_LENGTH 8192

int add_country_top(country_t countries[], const country_t country, size_t *length);
void print_countries(FILE *file_out, const country_t countries[], const size_t length);

#endif //__COUNTRIES_ARRAY_H__