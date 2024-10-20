#include "research.h"
#include "summ.h"
#include "inits.h"

void print_stats(const int *rows, const int *times_standart, const int *times_fast, const size_t length, const size_t percents)
{
    printf("Данные для %zu процентов заполнения\n", percents);

    printf("-------------------------------------------------------------\n");
    printf("| Размер матрицы | Обычные матрицы, мкс  |  CSC матрицы, мкс  |\n");
    printf("-------------------------------------------------------------\n");

    for (size_t i = 0; i < length; ++i)
    {
        printf("| %13d | %21d | %19d |\n", rows[i], times_standart[i], times_fast[i]);    
    }
    printf("-------------------------------------------------------------\n\n");
}


static void build_matrix(matrix_t *matrix, const size_t rows, const size_t columns, const size_t percents)
{
    size_t count = (double) (columns * rows) * ((double) percents / (double) 100);
    printf("\nDBG: DEF матрица. Будет заполнено %zu элементов из %zu\n\n", count, columns * rows);
    
    init_random(&matrix, rows, columns, count);
}

static void build_csc_matrix(csc_matrix_t *matrix, const size_t rows, const size_t columns, const size_t percents)
{
    size_t count = (double) (columns * rows) * ((double) percents / (double) 100);
    printf("\nDBG: CSC матрица. Будет заполнено %zu элементов из %zu\n\n", count, columns * rows);
    
    csc_init_random(&matrix, rows, columns, count);
}

static unsigned long long clock_time_one_summ_standart(const size_t rows, const size_t columns, const size_t percents)
{
    matrix_t left, right;
    unsigned long long cur_time = 0, summ_time = 0;

    left.rows = rows;
    left.columns = columns;
    matrix_alloc(&left);

    right.rows = rows;
    right.columns = columns;
    matrix_alloc(&right);

    build_matrix(&left, rows, columns, percents);
    build_matrix(&right, rows, columns, percents);

    for (size_t i = 0; i < REPS + 2; ++i)
    {
        cur_time = clock_time_summ_matrix_standart(&left, &right);
        if (i > 1)
            summ_time += cur_time;
    }

    matrix_clean(&left);
    matrix_clean(&right);

    return summ_time / REPS;
}

static unsigned long long clock_time_one_summ_fast(const size_t rows, const size_t columns, const size_t percents)
{
    csc_matrix_t left, right;
    size_t count = (double) (columns * rows) * ((double) percents / (double) 100);
    unsigned long long cur_time = 0, summ_time = 0;

    left.rows = rows;
    left.columns = columns;
    left.len_A = count;
    csc_matrix_alloc(&left);

    left.rows = rows;
    left.columns = columns;
    left.len_A = count;
    matrix_alloc(&right);

    build_csc_matrix(&left, rows, columns, percents);
    build_csc_matrix(&right, rows, columns, percents);

    for (size_t i = 0; i < REPS + 2; ++i)
    {
        cur_time = clock_time_summ_matrix_fast(&left, &right);
        if (i > 1)
            summ_time += cur_time;
    }

    csc_matrix_clean(&left);
    csc_matrix_clean(&right);

    return summ_time / REPS;
}

void make_research(const int *rows, const int *columns, const size_t length, const int *persents, const size_t per_length)
{
    //Массив, где каждый элемент - кол-во строк и столбцов в матрице. Хранит среднее время
    unsigned long long *times_standart = malloc(sizeof(unsigned long long) * (length));
    unsigned long long *times_fast = malloc(sizeof(unsigned long long) * (length));
    
    for (size_t i = 0; i < per_length; ++i)
    {
        for (size_t j = 0; j < length; ++j)
        {
            times_standart[j] = clock_time_one_summ_standart(rows[j], columns[j], persents[i]);
            times_fast[j] = clock_time_one_summ_fast(rows[j], columns[j], persents[i]);
        }
        print_stats(rows, times_standart, times_fast, length, persents[i]);
    }

    free(times_standart);
    free(times_fast);
}
