#ifndef __INITS_H__
#define __INITS_H__

#define BASE 9

#include "matrix.h"

void csc_matrix_alloc(csc_matrix_t *matrix);

void csc_init_random(csc_matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzero);

void csc_matrix_clean(csc_matrix_t *matrix);

void appropriate_csc_matrix(csc_matrix_t *dest, csc_matrix_t *src);

void matrix_alloc(matrix_t *matrix);

void init_random(matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzero);

void matrix_clean(matrix_t *matrix);

void appropriate_matrix(matrix_t *dest, matrix_t *src);

#endif //__INITS_H__
