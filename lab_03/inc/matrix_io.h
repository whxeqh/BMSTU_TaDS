#ifndef __MATRIX_IO_H__
#define __MATRIX_IO_H__

#include "errors.h"
#include "matrix.h"
#include <stdio.h>

#define MAX_FILENAME_LEN (100 + 1)

typedef enum
{
    ACT_READ_FILE = 1,
    ACT_READ_RANDOM,
    ACT_READ_CONSOLE
} act_read_e;

errors_e read_matrix(csc_matrix_t *matrix);

errors_e input_random(csc_matrix_t *matrix);

errors_e read_filename_and_open_file(FILE **file, const char *mode);

void print_matrix(csc_matrix_t *matrix, matrix_t *default_matrix, FILE *f);

void csc_free_matrix(csc_matrix_t *matrix);

void print_vectors(csc_matrix_t *matrix);

#endif //__MATRIX_IO_H__