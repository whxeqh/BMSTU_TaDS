#include "inits.h"
#include "matrix.h"
#include "errors.h"
#include "matrix_io.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))

/*
static void print_vectorss(int *A, size_t *IA, size_t *JA, size_t len_A, size_t columns)
{
    printf("A:  ");
    for (size_t i = 0; i < len_A; ++i)
        printf("%d ", A[i]);
    puts("\n");

    printf("IA: ");
    for (size_t i = 0; i < len_A; ++i)
        printf("%zu ", IA[i]);
    puts("\n");

    printf("JA: ");
    for (size_t i = 0; i < columns; ++i)
        printf("%zu ", JA[i]);
    puts("\n");
}

static void print_matrixx(int **A, size_t rows, size_t columns)
{
    puts("Matrix:");
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
            printf("%d ", A[i][j]);
        puts("");
    }
    puts("");
}*/

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
    matrix->matrix = calloc(matrix->rows, sizeof(size_t*));
    if (!matrix->matrix)
    {
        printf("Memory allocation error in file: %s\n Line: %d\n", __FILE__, __LINE__);
        exit(ERR_MEMORY);
    }

    for (size_t i = 0; i < matrix->rows; ++i)
    {
        matrix->matrix[i] = calloc(matrix->columns, sizeof(int));
        if (!matrix->matrix[i])
        {
            printf("Memory allocation error in file: %s\n Line: %d\n", __FILE__, __LINE__);
            exit(ERR_MEMORY);
        }
    }
}

void init_random(matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzero)
{
    int *A = malloc(sizeof(int) * nonzero);
    size_t *IA = malloc(sizeof(size_t) * nonzero);
    size_t *JA = malloc(sizeof(size_t) * nonzero);

    init_vectors_random(A, IA, JA, rows, columns, nonzero);
    //print_vectorss(A, IA, JA, nonzero, columns);

    for (size_t i = 0; i < nonzero; ++i)
        matrix->matrix[IA[i]][JA[i]] = A[i];

    //print_matrixx(matrix->matrix, matrix->rows, matrix->columns);
    
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
