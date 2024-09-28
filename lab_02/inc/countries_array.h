#ifndef __COUNTRIES_ARRAY_H__
#define __COUNTRIES_ARRAY_H__

#include <stdio.h>
#include <stddef.h>

#include "structures.h"

#define MAX_LENGTH 11000

int add_country_top(country_t countries[], const country_t country, size_t *length);

void print_countries(FILE *file_out, const country_t countries[], const size_t length);

void delete_in_array(country_t *countries, size_t *length, const size_t pos);

bool find_in_array(country_t *countries, const size_t length, char *field, size_t *pos);

int print_task_countries(const country_t countries[], const size_t length);

void print_countries_by_keys(const country_t *countries, const size_t length, const key_t *keys);

void print_keys(FILE *file_out, key_t *keys, const size_t keys_length);

void fill_keys(country_t *countries, const size_t length, key_t *keys);

#endif //__COUNTRIES_ARRAY_H__