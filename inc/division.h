#ifndef __DIVISION_H__
#define __DIVISION_H__

#include "big_nums.h"
#include <stdbool.h>

typedef struct
{
    bool flag;
    short exp;
} exp_t;

/** 
 * @brief Основная функция деления
 * @param divident делимое
 * @param divisor делитель
 * @return частное 
*/
bdouble_t div_big_numbers(bdouble_t *divident, bdouble_t *divisor);

#endif