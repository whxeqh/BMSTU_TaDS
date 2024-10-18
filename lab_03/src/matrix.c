#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))

static int **matrix_alloc(const size_t rows, const size_t columns)
{   
    int **matrix = NULL;
    errors_e rc = OK;

    matrix = calloc(rows, sizeof(int*));

    for (size_t i = 0; rc == OK && i < rows; ++i)
    {
        matrix[i] = calloc(columns, sizeof(int));
        if (!matrix[i])
        {
            free_matrix(matrix, rows);
            return NULL;
        }
    }

    return matrix;
}

matrix_t fill_matrix(csc_matrix_t *matrix)
{
    matrix_t cur;
    cur.columns = matrix->columns;
    cur.rows = matrix->rows;
    cur.matrix = matrix_alloc(cur.rows, cur.columns);
    if (!cur.matrix)
    {
        printf(RED "\nERROR WHILE ALLOCATE MEMORY FOR BIG MATRIX!\n\n" RESET);
        exit(ERR_MEMORY);
    }
    
    size_t cnt = 0;
    for (size_t j = 1; j < matrix->columns; j++)
    {
        for (size_t i = matrix->JA[j - 1]; i < matrix->JA[j]; i++)
        {
            cur.matrix[matrix->IA[i]][j - 1] = matrix->A[cnt++];
        }
    }
    
    for (size_t i = matrix->JA[matrix->columns - 1]; i < matrix->len_A; i++)
        cur.matrix[matrix->IA[i]][matrix->columns - 1] = matrix->A[cnt++];
    
    return cur;
}

void free_matrix(int **matrix, const size_t rows)
{
    for (size_t i = 0; i < rows; ++i)    
        free(matrix[i]);
    free(matrix);
}

errors_e sum_matrix_standart(matrix_t *summ, csc_matrix_t *pleft, csc_matrix_t *pright)
{
    matrix_t left = fill_matrix(pleft);
    matrix_t right = fill_matrix(pright);

    if (left.columns != right.columns || left.rows != right.rows)
        return ERR_RANGE;
    
    summ->rows = left.rows;
    summ->columns = left.columns;
    summ->matrix = matrix_alloc(left.rows, left.columns);
    
    for (size_t i = 0; i < summ->rows; ++i)
    {
        for (size_t j = 0; j < summ->columns; ++j)
        {
            summ->matrix[i][j] = left.matrix[i][j] + right.matrix[i][j];
        }
    }

    return OK;
}

errors_e sum_matrix_fast(csc_matrix_t *summ, csc_matrix_t *pleft, csc_matrix_t *pright)
{
    errors_e rc = OK;
    size_t length = MAX((int) pleft->len_A, (int) pright->len_A);
    

    if (pleft->columns != pright->columns || pleft->rows != pright->rows)
        return ERR_RANGE;

    summ->columns = pleft->columns;
    summ->rows = pleft->rows;

    summ->A = malloc(sizeof(int) * length);
    if (!summ->A)
        rc = ERR_MEMORY;

    summ->IA = malloc(sizeof(size_t) * length);
    if (!summ->IA)
        rc = ERR_MEMORY;
    
    summ->JA = malloc(sizeof(size_t) * summ->columns);
    if (!summ->JA)
        rc = ERR_MEMORY;

    /*for (size_t i = 0; i < summ->columns; ++i)
    {
        for (size_t)
    }*/

    return rc;
}