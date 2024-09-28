#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h>
#include "structures.h"

void bubble_sort_countries(country_t *countries, const int len);

void bubble_sort_keys(key_t *keys, const int len);

void flag_bubble_sort_countries(country_t *countries, const int len);

void flag_bubble_sort_keys(key_t *keys, const int len);


#endif //__SORT_H__