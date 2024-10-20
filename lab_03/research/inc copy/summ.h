#define _POSIX_C_SOURCE 199309L
#ifndef __SUMM_H__
#define __SUMM_H__

#include "time.h"
#include "matrix.h"

unsigned long long calc_elapsed_time(const struct timespec *beg, const struct timespec *end);

//---------------------------

void summ_matrix_standart(matrix_t *summ, matrix_t *pleft, matrix_t *pright);

unsigned long long clock_time_summ_matrix_standart(matrix_t *pleft, matrix_t *pright);

//-------------------------------

void summ_matrix_fast(csc_matrix_t *summ, csc_matrix_t *pleft, csc_matrix_t *pright);

unsigned long long clock_time_summ_matrix_fast(csc_matrix_t *pleft, csc_matrix_t *pright);


#endif //__SUMM_H__
