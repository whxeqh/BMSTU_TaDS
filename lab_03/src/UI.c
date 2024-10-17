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
    | 8) Произвести исследование                                 |\n\
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
    errors_e rc = OK;
    action_e act = ACT_UNKNOWN;

    csc_matrix_t left_csc_matrix, right_csc_matrix;
    matrix_t summ;
    
    static bool matrix_entered = false;
    static bool matrix_summed = false;
    
    print_menu();
    printf("\nВведите число от 0 до 9: ");
    fscanf(stdin, "%u", &act);
    if (!clear_buf())
        return ERR_IO;

    switch (act)
    {
        case ACT_EXIT:
            memset(&left_csc_matrix, 0, sizeof(csc_matrix_t));
            memset(&right_csc_matrix, 0, sizeof(csc_matrix_t));
            csc_free_matrix(&left_csc_matrix);
            csc_free_matrix(&right_csc_matrix);
            rc = EXIT;
            break;

        case ACT_PRINT_AUTOR_INFO:

            break;

        case ACT_READ_MATRIX:
            rc = read_matrix(&left_csc_matrix);
            if (rc == OK)
                rc = read_matrix(&right_csc_matrix);
            if (rc == OK)
            {
                printf(GREEN "\nМатрицы успешно считаны!\n\n" RESET);
                matrix_entered = true;
            }
            break;
            
        case ACT_PRINT_MATRIX:
            puts("\nМатрица №1:\n");
            print_matrix(&left_csc_matrix, NULL, stdout);
            puts("Матрица №2:\n");
            print_matrix(&right_csc_matrix, NULL, stdout);
            printf(GREEN "\nМатрицы успешно выведены!\n\n" RESET);
            break;
        
        case ACT_PRINT_SUM_MATRIX:
            if (!matrix_summed)
            {
                printf(RED "\nВы до этого не выполняли сложения матриц, чтобы выводить их)!\n\n" RESET);
                rc = ERR_ACT;
            }
            else
            {
                puts("");
                print_matrix(NULL, &summ, stdout);
                printf(GREEN "Сумма матриц успешно выведена!\n\n" RESET);
            }
            break;
        
        case ACT_PRINT_VECTORS:
            puts("\nВекторы для матрицы №1:\n");
            print_vectors(&left_csc_matrix);
            puts("Векторы для матрицы №2:\n");
            print_vectors(&right_csc_matrix);
            printf(GREEN "\nВекторы матриц успешно выведены!\n\n" RESET);
            break;

        case ACT_SUM_MATRIX_STANDART:
            if (!matrix_entered)
            {
                printf(RED "\nНе введены две матрицы !\n\n" RESET);
                rc = ERR_ACT;
            }
            else 
            {
                rc = sum_matrix_standart(&summ, &left_csc_matrix, &right_csc_matrix);
                if (rc == OK)
                {
                    printf(GREEN "\nМатрицы успешно сложены!\n\n" RESET);
                    matrix_summed = true;
                }
            }
            break;

        case ACT_SUM_MATRIX_FAST:

            break;

        case ACT_MAKE_RESEARCH:
            break;

        default:
            rc = ERR_ACT;
            break;
    }

    if (act == ACT_UNKNOWN)
        rc = ERR_ACT;

    return rc;
}