#ifndef BIG_NUMS_H__
#define BIG_NUMS_H__

#define MANTISA_LEN     42
#define EXPONENTA_LEN   5

#define BASE 10

#include "stddef.h"

typedef struct 
{
    char sign;                          // Знак числа
    int mantissa[MANTISA_LEN];          // Массив мантиссы 
    int exponent;                       // Экспонента
    size_t man_length;                  // Количество заполненных цифр в мантиссе
} bdouble_t;


void parce_numbers(char *int_num, char *real_num, bdouble_t *pint, bdouble_t *preal);

void reverse_mantissa(int *num, const size_t len);

void swap_nums(int *a, int *b);

void print_bdouble(const bdouble_t *num);

void delete_first_zeros(bdouble_t *num, const size_t len);

#endif
