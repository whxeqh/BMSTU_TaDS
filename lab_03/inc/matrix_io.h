#ifndef __MATRIX_IO_H__
#define __MATRIX_IO_H__

#include "errors.h"
#include "matrix.h"

#define MAX_FILENAME_LEN (20 + 1)

typedef enum
{
    ACT_READ_FILE = 1,
    ACT_READ_RANDOM,
    ACT_READ_CONSOLE
} act_read_e;

errors_e read_matrix(csc_matrix_t *matrix);

void print_matrix(csc_matrix_t *matrix);

void csc_free_matrix(csc_matrix_t *matrix);

void free_matrix(int **matrix, const size_t rows);

#endif //__MATRIX_IO_H__