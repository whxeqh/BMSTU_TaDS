#include "research.h"
#include "summ.h"
#include "inits.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))

static void print_time_stats(const int *rows, const int *columns, const unsigned long long *times_standart, const unsigned long long *times_fast, const size_t length)
{
    printf(BOLD CYAN "Таблица с данными о времени сложения матриц\n" RESET);
    printf("--------------------------------------------------------------------\n");
    printf("| "BOLD" Размер матрицы "RESET" | "BOLD" Обычные матрицы, мкс  "RESET" | "BOLD" CSC матрицы, мкс "RESET"  |\n");
    printf("--------------------------------------------------------------------\n");

    for (size_t i = 0; i < length; ++i)
    {
        printf("| %6d%6d     |    %10lld           |    %9lld        |\n", 
               rows[i], columns[i], times_standart[i], times_fast[i]);    
    }
    printf("--------------------------------------------------------------------\n\n");
}

static void print_memory_stats(const int *rows, const int *columns, const size_t *memory_standart, const size_t *memory_fast, const size_t length)
{
    printf(BOLD CYAN "\nТаблица с данными о объёме памяти для хранения матриц\n" RESET);
    printf("---------------------------------------------------------------------\n");
    printf("| "BOLD" Размер матрицы "RESET" | "BOLD" Обычные матрицы, байт  "RESET" | "BOLD" CSC матрицы, байт "RESET" |\n");
    printf("---------------------------------------------------------------------\n");

    for (size_t i = 0; i < length; ++i)
    {
        printf("| %6d%6d     |    %12ld          |    %9ld        |\n", 
               rows[i], columns[i], memory_standart[i], memory_fast[i]);    
    }
    printf("---------------------------------------------------------------------\n\n");
}

void print_stats(const int *rows, const int *columns, const size_t *memory_standart, const size_t *memory_fast, const unsigned long long *times_standart, const unsigned long long *times_fast, const size_t length, const size_t percents)
{
    printf(BOLD GREEN "\nДанные для %zu процентов заполнения\n\n" RESET, percents);
    print_time_stats(rows, columns, times_standart, times_fast, length);
    print_memory_stats(rows, columns, memory_standart, memory_fast, length);
    puts("\n");
}


static void build_matrix(matrix_t *matrix, const size_t rows, const size_t columns, const size_t percents)
{
    size_t count = (double) (columns * rows) * ((double) percents / (double) 100);
    //printf("\nDBG: DEF матрица. Будет заполнено %zu элементов из %zu. Процент: %zu\n\n", count, columns * rows, percents);
    
    init_random(matrix, rows, columns, count);
}

static void build_csc_matrix(csc_matrix_t *matrix, const size_t rows, const size_t columns, const size_t percents)
{
    size_t count = (double) (columns * rows) * ((double) percents / (double) 100);
    //printf("\nDBG: CSC матрица. Будет заполнено %zu элементов из %zu. Процент: %zu\n\n", count, columns * rows, percents);
    
    csc_init_random(matrix, rows, columns, count);
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
        
        //Чтобы обмануть оптимизатор
        int tmp = left.matrix[0][0];
        left.matrix[0][0] = right.matrix[0][1];
        right.matrix[0][1] = tmp;

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

    right.rows = rows;
    right.columns = columns;
    right.len_A = count;
    csc_matrix_alloc(&right);

    build_csc_matrix(&left, rows, columns, percents);
    build_csc_matrix(&right, rows, columns, percents);

    for (size_t i = 0; i < REPS + 2; ++i)
    {
        cur_time = clock_time_summ_matrix_fast(&left, &right);

        //Чтобы обмануть оптимизатор
        int tmp = left.A[0];
        left.A[0] = right.A[1];
        right.A[1] = tmp;

        size_t tmp2 = left.IA[0];
        left.IA[0] = right.IA[1];
        right.IA[1] = tmp2;

        size_t tmp3 = left.JA[0];
        left.JA[0] = right.JA[1];
        right.JA[1] = tmp3;

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
    size_t *memory_standart = malloc(sizeof(size_t) * (length));
    size_t *memory_fast = malloc(sizeof(size_t) * (length));


    for (size_t i = 0; i < per_length; ++i)
    {
        for (size_t j = 0; j < length; ++j)
        {
            //printf("Процент заполнения: %u, Размеры матрицы: %ux%u\n", persents[i], rows[j], columns[j]);
            times_standart[j] = clock_time_one_summ_standart(rows[j], columns[j], persents[i]);
            memory_standart[j] = rows[j] * columns[j] * sizeof(int);
            
            times_fast[j] = clock_time_one_summ_fast(rows[j], columns[j], persents[i]);

            size_t nonzero = (double) (columns[j] * rows[j]) * ((double) persents[i] / (double) 100);
            memory_fast[j] = nonzero * sizeof(int) * 2 + sizeof(int) * MAX((int)nonzero, (int)columns[j]);
        }
        print_stats(rows, columns, memory_standart, memory_fast, times_standart, times_fast, length, persents[i]);
    }

    free(times_standart);
    free(times_fast);
}
