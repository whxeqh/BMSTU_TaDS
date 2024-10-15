#include "errors.h"
#include "matrix_io.h"
#include "matrix.h"
#include "UI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    if (!fgets(buf, sizeof(buf), stdin))
        return ERR_IO;

    char *p = strchr(buf, '\n');
    if (!p)
        return ERR_BUF_TO_BIG;
    *p = '\0';

    return OK;
}

static errors_e read_filename_and_open_file(FILE **file)
{
    errors_e rc = OK;
    char filename[MAX_FILENAME_LEN];

    fprintf(stdout, "Введите название файла: ");
    rc = read_str(filename);
    if (rc != OK)
        return rc;

    *file = fopen(filename, "r");
    if (!file)
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

static errors_e matrix_alloc(matrix_t *matrix, FILE *file)
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
    matrix->columns = tmp_columns;
    matrix->len_A = tmp_nonzero;
    
    matrix->A = malloc(sizeof(int) * matrix->len_A);
    if (!matrix->A)
        rc = ERR_MEMORY;

    matrix->IA = malloc(sizeof(size_t) * matrix->len_A);
    if (!matrix->IA)
        rc = ERR_MEMORY;
    
    matrix->JA = malloc(sizeof(size_t) * matrix->len_A);
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

static void print_vectors(matrix_t *matrix)
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
    for (size_t i = 0; i < matrix->columns; ++i)
        printf("%zu ", matrix->JA[i]);
    puts("\n");
}

//Используется только для ввода с консоли
static void sort_vectors(matrix_t *matrix)
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

    for (size_t i = 1; i < matrix->len_A; i++)
    {
        size_t j = i;
        while (j < matrix->len_A && matrix->JA[j] != i)
            j++;
        
        if (j >= matrix->len_A) 
            matrix->JA[i] = matrix->JA[i - 1];
        else 
            matrix->JA[i] = j;
    }
}


static errors_e input_matrix(matrix_t *matrix, FILE *file_in)
{
    errors_e rc = OK;
    matrix_t tmp;

    rc = matrix_alloc(&tmp, file_in);
    if (rc == ERR_MEMORY)
        free_matrix(&tmp);
    
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
            printf("i = ");
            rc = read_cords(file_in, &row, 1, tmp.rows);
            if (rc != OK)
            {
                free_matrix(&tmp);
                return rc;
            }

            printf("j = ");
            rc = read_cords(file_in, &column, 1, tmp.columns);
            if (rc != OK)
            {
                free_matrix(&tmp);
                return rc;
            }

            printf("elem = ");
            if (fscanf(file_in, "%d", &elem) != 1)
                rc = ERR_IO;
            
            if (rc != OK)
            {
                free_matrix(&tmp);
                return rc;
            }

            tmp.A[i] = elem;
            tmp.IA[i] = row;
            tmp.JA[i] = column;

            puts("\n");
        }
    }

    else
    {
        size_t i = 0, j = 0;
        do
        {
            int cur;
            fscanf(file_in, "%d", &cur);
            if (cur)
            {
                tmp.A[j] = cur;
                tmp.IA[j] = i / tmp.rows;  
                tmp.JA[j] = i % tmp.columns;
                ++j;
            }
            ++i;
        } while (!feof(file_in));
    }
    
    memcpy(matrix, &tmp, sizeof(tmp));

    //size_t tmp_rows = matrix->rows;
    //size_t tmp_columns = matrix->columns;

    //matrix->columns = matrix->len_A;
    //matrix->rows = matrix->len_A;   
    
    sort_vectors(matrix);

    size_t *tmp_JA = realloc(matrix->JA, sizeof(size_t) * matrix->columns);
    if (!tmp_JA)
    {
        free_matrix(matrix);
        return ERR_MEMORY;
    }
    matrix->JA = tmp_JA;
    //matrix->columns = tmp_columns;
    //matrix->rows = tmp_rows;

    //realloc(matrix->IA, matrix->rows * sizeof(int));
    //realloc(matrix->JA, matrix->columns * sizeof(int));

    puts("\nDBG vectors start");
    print_vectors(matrix);
    puts("DBG vectors end");

    return rc;
}

errors_e read_matrix(matrix_t *matrix)
{
    errors_e rc = OK;
    act_read_e act_read;
    FILE *file_in = NULL;

    print_read_matrix_menu();
    fscanf(stdin, "%u", &act_read);
    clear_stdin_buf();

    switch (act_read)
    {
        case ACT_READ_FILE:
            rc = read_filename_and_open_file(&file_in);
            if (rc != OK)
                return rc;
            rewind(file_in);
            rc = input_matrix(matrix, file_in);
            break;
        case ACT_READ_RANDOM:

            break;
        case ACT_READ_CONSOLE:
            rc = input_matrix(matrix, stdin);
            break;
        default:
            rc = ERR_ACT;
            break;
    }

    if (act_read != ACT_READ_CONSOLE && rc != ERR_FILE)
        fclose(file_in);

    return rc;
}

void free_matrix(matrix_t *matrix)
{
    free(matrix->A);
    free(matrix->IA);
    free(matrix->JA);
    memset(matrix, 0, sizeof(*matrix));
}