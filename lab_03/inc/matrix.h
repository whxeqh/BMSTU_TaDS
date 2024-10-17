#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>

#include "errors.h"

typedef struct 
{
    int *A;
    size_t len_A;

    size_t *IA;
    size_t rows;

    size_t *JA;
    size_t columns;

} csc_matrix_t;

typedef struct 
{
    int **matrix;
    size_t rows;
    size_t columns;
} matrix_t;

matrix_t fill_matrix(csc_matrix_t *matrix);

void free_matrix(int **matrix, const size_t rows);

errors_e sum_matrix_standart(matrix_t *summ, csc_matrix_t *pleft, csc_matrix_t *pright);

#endif //__MATRIX_H__