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
    size_t zeros_after_dot = 0, zeros_before_dot = 0;
    size_t i = 0;
    if (num[i] == '-' || num[i] == '+')
        i++;
    
    pdouble->sign = (i ? '-' : '+');
    
    while(num[i] == '0')
        ++i;

    size_t dot_pos = strcspn(&num[i], ".");
    pdouble->exponent = dot_pos;
    while(dot_pos--)
    {
        pdouble->mantissa[pdouble->man_length++] = num[i++] - '0';
        zeros_before_dot++;
        if (num[i] == 'E' && dot_pos != 1)
        {
            size_t i = pdouble->man_length - 1;
            while(pdouble->mantissa[i] == 0)
            {
                pdouble->exponent++;
                pdouble->man_length--;
                i--;
            }
            break;
        }
    }
    if (num[i] == '.')
        i++; //Пропуск самой точки

    size_t exp_pos = strcspn(&num[i], "E");
    size_t digit = false;
    while(exp_pos--)
    {
        if (num[i] - '0' != 0)
            digit = true;
        if (num[i] - '0' == 0 && digit && !pdouble->man_length)
            zeros_after_dot++;
        else if (digit)
        { 
            pdouble->mantissa[pdouble->man_length++] = num[i] - '0';
        }
        i++;
    }

    i++; // Пропуск экспоненты
    pdouble->exponent = atoi(&num[i]) - zeros_after_dot + zeros_before_dot;
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
int parce_numbers(char *int_num, char *real_num, bdouble_t *pint, bdouble_t *preal)
{
    parce_int(int_num, pint);
    parce_double(real_num, preal);
    return OK;
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