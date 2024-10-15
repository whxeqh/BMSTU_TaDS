#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>

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


#include "errors.h"

#endif //__MATRIX_H__