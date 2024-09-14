#include "big_nums.h"
#include "errors.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

static void parce_int(const char *num, bdouble_t *pint)
{
    int i = 0, digits = 0;
    if (num[i] == '-' || num[i] == '+')
        i++;
    
    pint->sign = (i ? '-' : '+');
    
    while(num[i] == '0')
        i++;

    while(num[i] != '\0')
        pint->mantissa[digits++] = num[i++] - '0';

    i = digits - 1;
    while(pint->mantissa[i] == 0)
    {
        pint->exponent++;
        pint->man_length--;
        i--;
        digits--;
    }
    pint->exponent += digits;
    pint->man_length = digits;
}

static void parce_double(const char *num, bdouble_t *pdouble)
{
    bool dot = false;
    bdouble_t tmp;
    memset(&tmp, 0, sizeof(bdouble_t)); 
    size_t zeros_before_dot = 0;
    size_t i = 0;
    if (num[i] == '-' || num[i] == '+')
        i++;
    
    tmp.sign = (i ? '-' : '+');
    
    while(num[i] == '0')
        ++i;

    size_t dot_pos = strcspn(&num[i], ".");
    tmp.exponent = dot_pos;
    while(dot_pos--)
    {
        tmp.mantissa[tmp.man_length++] = num[i++] - '0';
        zeros_before_dot++;
        if (num[i] == 'E' && dot_pos != 1)
        {
            size_t i = tmp.man_length - 1;
            while(tmp.mantissa[i] == 0)
            {
                tmp.exponent++;
                tmp.man_length--;
                i--;
            }
            break;
        }
    }
    if (num[i] == '.')
    {
        dot = true;
        i++; //Пропуск самой точки
    }
    if (dot && (num[i] == '+' || num[i] == '-'))
        i++;

    size_t exp_pos = strcspn(&num[i], "E");
    size_t j = 0;
    bool flag = false;
    while(exp_pos--)
    {
        tmp.mantissa[tmp.man_length++] = num[i] - '0';
        if (num[i] - '0' != 0)
            flag = true;
        j++, i++;
    }
    if (!flag)
    {
        tmp.man_length -= j; 
        tmp.exponent += j;
        delete_first_zeros(&tmp, j);
    }
    if (tmp.man_length == 0)
        tmp.man_length++;

    i++; // Пропуск экспоненты
    tmp.exponent = atoi(&num[i]) + zeros_before_dot;

    memcpy(pdouble, &tmp, sizeof(bdouble_t));
}


void delete_first_zeros(bdouble_t *num, const size_t len)
{
    for (size_t i = len; i < num->man_length; i++)
        num->mantissa[i - len] = num->mantissa[i];
    for (size_t i = num->man_length - len; i < num->man_length; i++)
        num->mantissa[i] = 0;
}

void reverse_mantissa(int *mantissa, const size_t len)
{
    for(size_t i = 0, j = len - 1; i < j; ++i, --j)
        swap_nums(&mantissa[i], &mantissa[j]);
}

void swap_nums(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// 1213.000311130e-3134
// 1234.567 = 0.1234567E4
// 12.543 = 0.12543E2
void parce_numbers(char *int_num, char *real_num, bdouble_t *pint, bdouble_t *preal)
{
    parce_int(int_num, pint);
    parce_double(real_num, preal);
}

void print_bdouble(const bdouble_t *num)
 {
    if (num->exponent > 99999)
        printf("Достигнута машинная бесконечность!\n");
    else if (num->exponent < -99999)
        printf("Достигнут машинный ноль!\n");
    
    else 
    {
        printf("%c0.", num->sign);
        for (size_t i = 0; i < num->man_length; ++i)
            printf("%d", num->mantissa[i]);

        if (num->exponent > 0)
            printf("E+%d\n", num->exponent);
        else
            printf("E%d\n", num->exponent);
    }
}