#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "UI.h"
#include "errors.h"
#include "matrix_io.h"

static void print_menu(void)
{
    fprintf(stdout,"\n\
    Выберите одно из слудующий действий:                         \n\
    _____________________________________________________________\n\
    | 0) Выход                                                   |\n\
    | 1) Вывести информацию об авторе                            |\n\
    | 2) Считать матрицы                                         |\n\
    | 3) Вывести матрицы в консоль                               |\n\
    | 4) Вывести результат сложения матриц в консоль             |\n\
    | 5) Вывести вектора (A, IA, JA) матриц в консоль            |\n\
    | 6) Сложить две матрицы стандартным алгоритмом              |\n\
    | 7) Сложить две матрицы усовершенствованным алгоритмом      |\n\
    | 8) Записать сумму матриц в файл                            |\n\
    | 9) Произвести исследование                                 |\n\
    |____________________________________________________________|\n\n");
}

bool clear_buf(void)
{
    bool ans = true;
    char c;
    while((c = getc(stdin)) != EOF && c != '\n')
    {
        //printf("c = %c; isspcace = %d\n", c, isspace(c));
        if (!isspace(c))
            ans = false;
    }
    return ans;
}

errors_e main_menu(void)
{
    FILE *f = NULL;
    errors_e rc = OK;
    action_e act = ACT_UNKNOWN;

    static csc_matrix_t left_csc_matrix, right_csc_matrix, summ_fast;
    static matrix_t summ_standart;
    
    static bool matrix1_entered = false;
    static bool matrix2_entered = false;
    static bool matrix_summed_standart = false;
    static bool matrix_summed_fast = false;
    
    print_menu();
    printf("\nВведите число от 0 до 9: ");
    fscanf(stdin, "%u", &act);
    if (!clear_buf())
        return ERR_IO;

    switch (act)
    {
        case ACT_EXIT:
            if (matrix1_entered)
                csc_free_matrix(&left_csc_matrix);
            if (matrix2_entered)
                csc_free_matrix(&right_csc_matrix);
            if (matrix_summed_fast)
                csc_free_matrix(&summ_fast);
            if (matrix_summed_standart)
                free_matrix(summ_standart.matrix, summ_standart.rows);

            rc = EXIT;
            break;

        case ACT_PRINT_AUTOR_INFO:
            puts("Палладий Евгений. ИУ7-31Б. 16 вариант");
            break;

        case ACT_READ_MATRIX:
            if (matrix1_entered)
                csc_free_matrix(&left_csc_matrix);
            if (matrix2_entered)
                csc_free_matrix(&right_csc_matrix);
            matrix_summed_standart = false;
            matrix_summed_fast = false;
            rc = read_matrix(&left_csc_matrix);
            if (rc == OK)
            {
                rc = read_matrix(&right_csc_matrix);
                matrix1_entered = true;
            }
            if (rc == OK)
            {
                printf(GREEN "\nМатрицы успешно считаны!\n\n" RESET);
                matrix2_entered = true;
            }
            break;
            
        case ACT_PRINT_MATRIX:
            if (!matrix1_entered || !matrix2_entered)
                printf(RED "\nДве матрицы не введены!\n\n" RESET);
            else 
            {
                puts("\nМатрица №1:\n");
                print_matrix(&left_csc_matrix, NULL, stdout);
                puts("Матрица №2:\n");
                print_matrix(&right_csc_matrix, NULL, stdout);
                printf(GREEN "\nМатрицы успешно выведены!\n\n" RESET);
            }
            break;
        
        case ACT_PRINT_SUM_MATRIX:
            if (!matrix_summed_standart && !matrix_summed_fast)
            {
                printf(RED "\nВы до этого не выполняли сложения матриц, чтобы выводить их)!\n\n" RESET);
                rc = ERR_ACT;
            }
            else
            {
                puts("");
                //puts("\nМатрица в обычном представлении");
                if (matrix_summed_fast)
                    print_matrix(&summ_fast, &summ_standart, stdout);
                else 
                    print_matrix(NULL, &summ_standart, stdout);
                //puts("\nМатрица в разреженном представлении (CSC)");
                //print_vectors(&summ_standart);
                printf(GREEN "Сумма матриц успешно выведена!\n\n" RESET);
            }
            break;
        
        case ACT_PRINT_VECTORS:
            if (!matrix1_entered || !matrix2_entered)
                printf(RED "\nДве матрицы не введены!\n\n" RESET);
            else 
            {
                puts("\nВекторы для матрицы №1:\n");
                print_vectors(&left_csc_matrix);
                puts("\nВекторы для матрицы №2:\n");
                print_vectors(&right_csc_matrix);
                if (matrix_summed_fast)
                {
                    puts("\nВекторы для матрицы сумм:\n");
                    print_vectors(&summ_fast);
                }
                printf(GREEN "\nВекторы матриц успешно выведены!\n\n" RESET);
            }
            break;

        case ACT_SUM_MATRIX_STANDART:
            if (!matrix1_entered && !matrix2_entered)
            {
                printf(RED "\nНе введены две матрицы !\n\n" RESET);
                rc = ERR_ACT;
            }
            else 
            {
                //free_matrix(summ_standart.matrix, summ_standart.rows);
                rc = sum_matrix_standart(&summ_standart, &left_csc_matrix, &right_csc_matrix);
                if (rc == OK)
                {
                    printf(GREEN "\nМатрицы успешно сложены!\n\n" RESET);
                    matrix_summed_standart = true;
                }
            }
            break;

        case ACT_SUM_MATRIX_FAST:
            if (!matrix1_entered && !matrix2_entered)
            {
                printf(RED "\nНе введены две матрицы !\n\n" RESET);
                rc = ERR_ACT;
            }
            else 
            {
                //csc_free_matrix(&summ_fast);
                rc = sum_matrix_fast(&summ_fast, &left_csc_matrix, &right_csc_matrix);
                if (rc == OK)
                {
                    printf(GREEN "\nМатрицы успешно сложены!\n\n" RESET);
                    matrix_summed_fast = true;
                }
                //print_vectors(&summ_fast);
            }
            break;

        case ACT_MAKE_RESEARCH:
            printf(GREEN "Напишите в консоль make research и будет чудо!\n" RESET);
            break;

        case ACT_PRINT_SUMM_MATRIX_IN_FILE:
            if (!matrix_summed_standart && !matrix_summed_fast)
            {
                printf(RED "\nВы до этого не выполняли сложения матриц, чтобы выводить их)!\n\n" RESET);
                rc = ERR_ACT;
            }
            else
            {
                rc = read_filename_and_open_file(&f, "w");
                if (rc == OK)
                {
                    puts("");
                    print_matrix(NULL, &summ_standart, f);
                    printf(GREEN "Сумма матриц успешно записана в файл!\n\n" RESET);
                    fclose(f);
                }
            }
            break;
        default:
            rc = ERR_ACT;
            break;
    }

    if (act == ACT_UNKNOWN)
        rc = ERR_ACT;

    return rc;
}