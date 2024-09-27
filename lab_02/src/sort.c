#include "sort.h"
#include <string.h>

static void uni_swap(void *left, void *right, const size_t el_size)
{
    char tmp[el_size];
    memcpy(tmp, left, el_size);
    memcpy(left, right, el_size);
    memcpy(right, tmp, el_size);
}

static int uni_cmp(const void *left, const void *right, const size_t el_size)
{
    const char *l = left;
    const char *r = right;
    for (size_t i = 0; i < el_size; ++i)
    {
        if (*l != *r)
            return *l - *r;
        l++;
        r++;
    }

    return 0;
}

void bubble_sort_countries(country_t *countries, const size_t len)
{
    for (size_t i = 0; i < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (uni_cmp(&countries[i].capital, &countries[j].capital, sizeof(countries[i].capital)) > 0)
                uni_swap(&countries[i], &countries[j], sizeof(country_t));
}

void bubble_sort_keys(key_t *keys, const size_t len)
{
    for (size_t i = 0; i < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (uni_cmp(keys[i].capital, &keys[j].capital, sizeof(keys[i].capital)) > 0)
                uni_swap(&keys[i], &keys[j], sizeof(key_t));
}

void flag_bubble_sort_countries(country_t *countries, const size_t len)
{
    bool flag = false;
    for (size_t i = 0; i < len; ++i)
    {
        for (size_t j = i + 1; j < len; ++j)
            if (uni_cmp(countries[i].capital, &countries[j].capital, sizeof(countries[i].capital)) > 0)
            {
                flag = true;
                uni_swap(&countries[i], &countries[j], sizeof(country_t));
            }  
        if (!flag)
            return;
    }    
}

void flag_bubble_sort_keys(key_t *keys, const size_t len)
{
    bool flag;
    int i = len - 1;
    do
    {
        flag = false;
        for (int j = 0; j < i; ++j)
        {
            if (uni_cmp(keys[j].capital, &keys[j + 1].capital, sizeof(keys[j].capital)) > 0)
            {
                flag = true;
                uni_swap(&keys[j], &keys[j + 1], sizeof(key_t));
            }
        }
        --i;
    } while (flag);
}
/*
void insertion_countries(country_t *countries, const size_t len)
{
    
}

void insertion_sort_keys(key_t *keys, const size_t len)
{
    
}
*/