#include "sort.h"
#include "country.h"
#include "countries_array.h"
#include <string.h>

static void uni_swap(void *left, void *right, const size_t el_size)
{
    char tmp[el_size];
    memcpy(tmp, left, el_size);
    memcpy(left, right, el_size);
    memcpy(right, tmp, el_size);
}

// Сравнение строк через strcmp
void bubble_sort_countries(country_t *countries, const int len)
{
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < len - 1; ++j)
            if (strcmp(countries[j].capital, countries[j + 1].capital) > 0)
                uni_swap(&countries[j], &countries[j + 1], sizeof(country_t));
}

void bubble_sort_keys(key_t *keys, const int len)
{
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < len - 1; ++j)
            if (strcmp(keys[j].capital, keys[j + 1].capital) > 0)
                uni_swap(&keys[j], &keys[j + 1], sizeof(key_t));
}

void flag_bubble_sort_keys(key_t *keys, const int len)
{
    bool flag;
    int i = 0;
    do
    {
        flag = false;
        for (int j = 0; j < len - i - 1; ++j)
        {
            if (strcmp(keys[j].capital, keys[j + 1].capital) > 0)
            {
                flag = true;
                uni_swap(&keys[j], &keys[j + 1], sizeof(key_t));
            }
        }
        i++;
    } while (flag);
}

void flag_bubble_sort_countries(country_t *countries, const int len)
{
    bool flag;
    int i = 0;
    do
    {
        flag = false;
        for (int j = 0; j < len - i - 1; ++j) // Исправлено
        {
            if (strcmp(countries[j].capital, countries[j + 1].capital) > 0)
            {
                flag = true;
                uni_swap(&countries[j], &countries[j + 1], sizeof(country_t));
            }
        }
        i++;
    } while (flag); 
}
