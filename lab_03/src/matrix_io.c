#include "errors.h"
#include "matrix_io.h"
#include "matrix.h"
#include "UI.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define BASE 9

static void print_read_matrix_menu(void)
{
    fprintf(stdout,"\n\
    Выберите, как заполнить матрицу:\n\
    __________________________________\n\
    | 1) Из файла                    |\n\
    | 2) Случайным образом           |\n\
    | 3) Из консоли                  |\n\
    |________________________________|\n\n");

    fprintf(stdout, "Введите число от 1 до 3: ");
}

static errors_e read_str(char *buf)
{
    if (!fgets(buf, MAX_FILENAME_LEN, stdin))
        return ERR_IO;

    char *p = strchr(buf, '\n');
    if (!p)
        return ERR_BUF_TO_BIG;
    *p = '\0';

    return OK;
}

errors_e read_filename_and_open_file(FILE **file, const char *mode)
{
    errors_e rc = OK;
    char filename[MAX_FILENAME_LEN];

    fprintf(stdout, "Введите название файла: ");
    rc = read_str(filename);
    if (rc != OK)
        return rc;

    *file = fopen(filename, mode);
    if (!(*file))
        return ERR_FILE;
    
    return rc;
}

static errors_e get_count_nonzero_elements(FILE *file, size_t *nz)
{
    errors_e rc = OK;
    size_t ans = 0;
    int tmp;
    size_t cnt = 0;

    //for (size_t i = 0; i < )
    do
    {
        if (fscanf(file, "%d\n", &tmp) != 1)
            return ERR_IO;
        cnt++;
        if (tmp != 0)
            ans++;
    } while (!feof(file));
    
    if (rc == OK)
        *nz = ans;
    
    rewind(file);

    int tmp1, tmp2;
    fscanf(file, "%d%d", &tmp1, &tmp2);
    if (cnt > (size_t) tmp1 * tmp2)
        rc = ERR_RANGE;
    return rc;
}

static errors_e csc_matrix_alloc(csc_matrix_t *matrix, FILE *file)
{   
    errors_e rc = OK;

    int tmp_rows, tmp_columns;
    size_t tmp_nonzero;

    if (file == stdin)
        printf("Введите количество строк: ");
    if (fscanf(file, "%d", &tmp_rows) != 1)
        return ERR_IO;
    if (tmp_rows < 1)
        return ERR_RANGE;

    if (file == stdin)
        printf("Введите количество столбцов: ");
    if (fscanf(file, "%d", &tmp_columns) != 1)
        return ERR_IO;
    if (tmp_columns < 1)
        return ERR_RANGE;

    if (file != stdin)
    {
        rc = get_count_nonzero_elements(file, &tmp_nonzero);
        if (rc != OK)
            return ERR_IO;
    }
    else
    {
        if (file == stdin)
            printf("Введите количество ненулевых элементов: ");
        if (fscanf(file, "%zu", &tmp_nonzero) != 1)
            return ERR_IO;
        if (tmp_nonzero == 0 || tmp_nonzero > (size_t) tmp_columns * tmp_rows)
            return ERR_RANGE;
    }


    matrix->rows = tmp_rows;
    matrix->columns = tmp_columns + 1;
    matrix->len_A = tmp_nonzero;

    //printf("\nDBG: rows = %d columns = %d nonzero = %ld\n",tmp_rows, tmp_columns, tmp_nonzero);
        
    matrix->A = malloc(sizeof(int) * matrix->len_A);
    if (!matrix->A)
        rc = ERR_MEMORY;

    matrix->IA = malloc(sizeof(size_t) * matrix->len_A);
    if (!matrix->IA)
        rc = ERR_MEMORY;
    
    matrix->JA = malloc(sizeof(size_t) * MAX((int) tmp_nonzero, (int) tmp_columns) + sizeof(size_t));
    if (!matrix->JA)
        rc = ERR_MEMORY;


    return rc;
}

static errors_e read_cords(FILE *file_in, size_t *ans, const int low, const int high)
{
    errors_e rc = OK;
    int tmp;

    if (fscanf(file_in, "%d", &tmp) != 1)
        rc = ERR_IO;
    if (rc == OK && (tmp < low || tmp > high))
        rc = ERR_RANGE;

    *ans = tmp - 1;
    return rc;
}

static void swap(void *left, void *right, const size_t el_size)
{
    char tmp[el_size];
    memcpy(tmp, left, el_size);
    memcpy(left, right, el_size);
    memcpy(right, tmp, el_size);
}

void print_vectors(csc_matrix_t *matrix)
{
    printf("A:  ");
    for (size_t i = 0; i < matrix->len_A; ++i)
        printf("%d ", matrix->A[i]);
    puts("\n");

    printf("IA: ");
    for (size_t i = 0; i < matrix->len_A; ++i)
        printf("%zu ", matrix->IA[i]);
    puts("\n");

    printf("JA: ");
    for (size_t i = 0; i < matrix->columns/*MAX(matrix->len_A, matrix->columns)*/; ++i)
        printf("%zu ", matrix->JA[i]);
    puts("\n");
}

//Используется только для ввода с консоли
void sort_vectors(csc_matrix_t *matrix)
{

    //puts("До сортировки");
    //print_vectors(matrix);

    //Сначала сортирую столбцы
    for (size_t i = 0; i < matrix->len_A; i++)
        for (size_t j = i + 1; j < matrix->len_A; j++)
            if (matrix->JA[i] > matrix->JA[j])
            {
                swap(&matrix->JA[j], &matrix->JA[i], sizeof(matrix->JA[j]));
                swap(&matrix->IA[j], &matrix->IA[i], sizeof(matrix->IA[j]));
                swap(&matrix->A[j], &matrix->A[i], sizeof(matrix->A[j]));
            }
    
    //puts("После сортировки столбцов");
    //print_vectors(matrix);
    
    //Потом сортирую строки
    for (size_t i = 0; i < matrix->len_A; i++)
        for (size_t j = i + 1; j < matrix->len_A && matrix->JA[i] == matrix->JA[j]; j++)
            if (matrix->IA[i] > matrix->IA[j])
            {
                swap(&matrix->JA[j], &matrix->JA[i], sizeof(matrix->JA[j]));
                swap(&matrix->IA[j], &matrix->IA[i], sizeof(matrix->IA[j]));
                swap(&matrix->A[j], &matrix->A[i], sizeof(matrix->A[j]));
            }
    
    //puts("После сортировки строк");
    //print_vectors(matrix);


    size_t *tmp_JA = malloc(sizeof(size_t) * matrix->columns);
    if (!tmp_JA)
        exit(1);

    size_t cur = 0, next = 0;
    tmp_JA[cur] = 0;
    //matrix->JA[0] = 0;
    for (size_t i = 1; i < matrix->columns; ++i)
    {  
        size_t j;
        bool is_column = false;
        for (j = 0; !is_column && j < matrix->len_A; ++j)
        {
            if (matrix->JA[j] == i)
            {
                is_column = true;
                cur = j;
                next = matrix->len_A;
            }
        }
        for (size_t k = 0; k < matrix->len_A; k++)
                if (matrix->JA[k] >= i + 1)
                {
                    next = k;
                    break;
                }
        if (!is_column)
            tmp_JA[i] = next;
        else 
            tmp_JA[i] = cur;
    }

    //printf("COLUMNS = %zu\n", matrix->columns);
    memcpy(matrix->JA, tmp_JA, sizeof(size_t) * matrix->columns);
    matrix->JA[matrix->columns - 1] = matrix->len_A;
    //matrix->columns = matrix->columns;
    free(tmp_JA);

    //puts("В конце:");
    //print_vectors(matrix);
    /*
    for (size_t i = 1; i < matrix->len_A; i++)
    {
        size_t j = i;
        while (j < matrix->len_A && matrix->JA[j] != i)
            j++;
        
        if (j >= matrix->len_A) 
            matrix->JA[i] = matrix->JA[i - 1];
        else 
            matrix->JA[i] = j;
    }*/
}


static errors_e input_matrix(csc_matrix_t *matrix, FILE *file_in)
{
    errors_e rc = OK;
    csc_matrix_t tmp;

    rc = csc_matrix_alloc(&tmp, file_in);
    if (rc == ERR_MEMORY)
        csc_free_matrix(&tmp);
    
    if (rc != OK)
        return rc;

    if (file_in == stdin)
    {   
        size_t row, column;
        int elem;
        printf("Вводите строки в диапазоне от 1 до %zu и столбцы в диапазоне от 1 до %zu\n\n", tmp.rows, tmp.columns);
        for (size_t i = 0; i < tmp.len_A && rc == OK; i++)
        {
            printf("Элемент №%zu\n", i + 1);
            bool flag = true;
            while (flag)
            {
                flag = false;
                printf("i = ");
                rc = read_cords(file_in, &row, 1, tmp.rows);
                if (rc != OK)
                {
                    csc_free_matrix(&tmp);
                    return rc;
                }

                printf("j = ");
                rc = read_cords(file_in, &column, 1, tmp.columns);
                if (rc != OK)
                {
                    csc_free_matrix(&tmp);
                    return rc;
                }

                for (size_t j = 0; j < i; ++j)
                    if (tmp.IA[j] == row && tmp.JA[j] == column)
                        flag = true;
                if (flag)
                    printf("\nКоординаты должны быть разными! Повторите ввод элемента №%zu\n", i + 1);
            }
            
            printf("elem = ");
            if (fscanf(file_in, "%d", &elem) != 1)
                rc = ERR_IO;
            
            if (rc != OK)
            {
                csc_free_matrix(&tmp);
                return rc;
            }
            //clear_buf();
            tmp.A[i] = elem;
            tmp.IA[i] = row;
            tmp.JA[i] = column;

            puts("\n");
        }
    }

    else
    {
        size_t j = 0;
        for (size_t i = 0; i < tmp.rows * (tmp.columns - 1); i++)
        {
            int cur;
            fscanf(file_in, "%d", &cur);
            if (cur && j < tmp.len_A)
            {
                tmp.A[j] = cur;
                tmp.IA[j] = i / (tmp.columns - 1);  
                tmp.JA[j] = i % (tmp.columns - 1);
                ++j;
            }
        }
    }
    
    memcpy(matrix, &tmp, sizeof(tmp));

    //size_t tmp_rows = matrix->rows;
    //size_t tmp_columns = matrix->columns;

    //matrix->columns = matrix->len_A;
    //matrix->rows = matrix->len_A;   
    
    sort_vectors(matrix);

    //size_t *tmp_JA = realloc(matrix->JA, sizeof(size_t) * matrix->columns);
    //if (!tmp_JA)
    //{
    //    free_matrix(matrix);
    //    return ERR_MEMORY;
    //}
    //matrix->JA = tmp_JA;
    //matrix->columns = tmp_columns;
    //matrix->rows = tmp_rows;

    //realloc(matrix->IA, matrix->rows * sizeof(int));
    //realloc(matrix->JA, matrix->columns * sizeof(int));

    //puts("\nDBG vectors start");
    //print_vectors(matrix);
    //puts("DBG vectors end");

    return rc;
}

errors_e input_random(csc_matrix_t *matrix)
{
    errors_e rc = OK;

    int tmp_rows, tmp_columns, percents;

    printf("Введите количество строк: ");
    if (scanf("%d", &tmp_rows) != 1)
        return ERR_IO;
    if (tmp_rows < 1)
        return ERR_RANGE;

    printf("Введите количество столбцов: ");
    if (scanf("%d", &tmp_columns) != 1)
        return ERR_IO;
    if (tmp_columns < 1)
        return ERR_RANGE;

    //csc_matrix_alloc(matrix, NULL);

    printf("Введите процентное соотношение заполнения матрицы: ");
    if (scanf("%d", &percents) != 1)
        return ERR_IO;
    if (percents < 0 || percents > 100)
        return ERR_RANGE;
    

    size_t count = (double) (tmp_columns * tmp_rows) * ((double) percents / (double) 100);
    printf("Будет заполнено %zu элементов из %d\n\n", count, tmp_columns * tmp_rows);

    matrix->rows = tmp_rows;
    matrix->columns = tmp_columns + 1;
    matrix->len_A = count;

    //printf("\nDBG: rows = %d columns = %d nonzero = %ld\n",tmp_rows, tmp_columns, count);
        
    matrix->A = malloc(sizeof(int) * matrix->len_A);
    if (!matrix->A)
        rc = ERR_MEMORY;

    matrix->IA = malloc(sizeof(size_t) * matrix->len_A);
    if (!matrix->IA)
        rc = ERR_MEMORY;
    
    matrix->JA = malloc(sizeof(size_t) * MAX((int) (count + 1), (int) (tmp_columns + 1)));
    if (!matrix->JA)
        rc = ERR_MEMORY;

    printf("count = %zu\n", count);
    for (size_t i = 0; rc == OK && i < count; i++)
    {
        matrix->A[i] = rand() % BASE + 1;
        bool flag = true;
        while (flag)
        {
            flag = false;
            matrix->IA[i] = rand() % tmp_rows;
            matrix->JA[i] = rand() % tmp_columns;
            for (size_t j = 0; j < i; ++j)
                if (matrix->IA[j] == matrix->IA[i] && matrix->JA[j] == matrix->JA[i])
                    flag = true;
        }
    
    }
    sort_vectors(matrix);
    return rc;
}

errors_e read_matrix(csc_matrix_t *matrix)
{
    errors_e rc = OK;
    act_read_e act_read;
    FILE *file_in = NULL;

    print_read_matrix_menu();
    if (fscanf(stdin, "%u", &act_read) != 1)
        return ERR_IO;
    if (!clear_buf())
        return ERR_IO;

    switch (act_read)
    {
        case ACT_READ_FILE:
            rc = read_filename_and_open_file(&file_in, "r");
            if (rc != OK)
                return rc;
            rewind(file_in);
            rc = input_matrix(matrix, file_in);
            if (rc == OK)
                printf(GREEN "\nМатрица усешно считаны\n" RESET);
            break;
        case ACT_READ_RANDOM:
            srand(time(NULL));
            rc = input_random(matrix);
            if (rc == OK)
                printf(GREEN "\nМатрица усешно заполнена случайными числами\n" RESET);
            break;
        case ACT_READ_CONSOLE:
            rc = input_matrix(matrix, stdin);
            if (rc == OK)
                printf(GREEN "\nМатрица усешно считана\n" RESET);
            break;
        default:
            rc = ERR_ACT;
            break;
    }

    if (act_read == ACT_READ_FILE && file_in != NULL)
        fclose(file_in);

    return rc;
}

void print_matrix(csc_matrix_t *matrix, matrix_t *default_matrix, FILE *f)
{
    matrix_t tmp;
    if (matrix != NULL)
    {
        default_matrix = &tmp;
        *default_matrix = fill_matrix(matrix);
    }
    //default_matrix->columns--;

    if (f != stdout)
        fprintf(f, "%zu %zu\n", default_matrix->rows, default_matrix->columns);

    for (size_t i = 0; i < default_matrix->rows; ++i)
    {
        for (size_t j = 0; j < default_matrix->columns - 1; ++j)
        {
                fprintf(f, "%d ", default_matrix->matrix[i][j]);
        }
        fprintf(f, "\n");
    }
    puts("");

    if (matrix != NULL)
        free_matrix(default_matrix->matrix, default_matrix->rows);
}


void csc_free_matrix(csc_matrix_t *matrix)
{
    free(matrix->A);
    free(matrix->IA);
    free(matrix->JA);
    memset(matrix, 0, sizeof(*matrix));
}