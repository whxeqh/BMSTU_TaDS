#include "inits.h"
#include "matrix.h"
#include "errors.h"
#include "matrix_io.h"
#include <stdbool.h>
#include <string.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))

static void init_vectors_random(int *A, size_t *IA, size_t *JA, const size_t rows, const size_t columns, const size_t nonzero)
{
    for (size_t i = 0; i < nonzero; i++)
    {
        A[i] = rand() % BASE + 1;
        bool flag = true;
        while (flag)
        {
            flag = false;
            IA[i] = rand() % rows;
            JA[i] = rand() % columns;
            for (size_t j = 0; j < i; ++j)
                if (IA[j] == IA[i] && JA[j] == JA[i])
                    flag = true;
        }
    }
}

void csc_matrix_alloc(csc_matrix_t *matrix)
{
    
    matrix->A = malloc(sizeof(int) * matrix->len_A);
    if (!matrix->A)
    {
        printf("Memory allocation error in file: %s\n Line: %d\n", __FILE__, __LINE__);
        exit(ERR_MEMORY);
    }

    matrix->IA = malloc(sizeof(size_t) * matrix->len_A);
    if (!matrix->IA)
    {
        printf("Memory allocation error in file: %s\n Line: %d\n", __FILE__, __LINE__);
        exit(ERR_MEMORY);
    }
    
    matrix->JA = malloc(sizeof(size_t) * MAX((int) (matrix->len_A + 1), (int) (matrix->columns + 1)));
    if (!matrix->JA)
    {
        printf("Memory allocation error in file: %s\n Line: %d\n", __FILE__, __LINE__);
        exit(ERR_MEMORY);
    }
}

void csc_init_random(csc_matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzero)
{
    init_vectors_random(matrix->A, matrix->IA, matrix->JA, rows, columns, nonzero);
    sort_vectors(matrix);
}

void csc_matrix_clean(csc_matrix_t *matrix)
{
    free(matrix->A);
    free(matrix->IA);
    free(matrix->JA);
}

void matrix_alloc(matrix_t *matrix)
{
    matrix->matrix = malloc(sizeof(int) * matrix->columns * matrix->rows);
    if (!matrix->matrix)
    {
        printf("Memory allocation error in file: %s\n Line: %d\n", __FILE__, __LINE__);
        exit(ERR_MEMORY);
    }
}

void init_random(matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzero)
{
    int *A = malloc(sizeof(int) * nonzero);
    size_t *IA = malloc(sizeof(int) * nonzero);
    size_t *JA = malloc(sizeof(int) * nonzero);

    init_vectors_random(A, IA, JA, rows, columns, nonzero);

    for (size_t i = 0; i < nonzero; ++i)
        matrix->matrix[IA[i]][JA[i]] = A[i];

    free(A);
    free(IA);
    free(JA);
}

void matrix_clean(matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->rows; ++i)
        free(matrix->matrix[i]);
    free(matrix->matrix);
}
