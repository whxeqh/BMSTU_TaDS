#ifndef __DIVISION_H__
#define __DIVISION_H__

#include "big_nums.h"


/** 
 * @brief Основная функция деления
 * @param divident делимое
 * @param divisor делитель
 * @return частное 
*/
bdouble_t div_big_numbers(const bdouble_t *divident, bdouble_t *divisor);

#endif