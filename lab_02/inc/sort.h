#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h>
#include "structures.h"

typedef void (*uni_cmp_t)(void *left, void *right);

void buble_sort_countries(country_t *countries, const size_t len);

void buble_sort_keys(key_t *keys, const size_t len);

void flag_buble_sort_countries(country_t *countries, const size_t len);

void flag_buble_sort_keys(key_t *keys, const size_t len);

void insertion_countries(country_t *countries, const size_t len);

void insertion_sort_keys(key_t *keys, const size_t len);

#endif //__SORT_H__