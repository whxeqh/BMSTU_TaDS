#include <stdio.h>
#include <ctype.h>
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

void clear_stdin_buf(void)
{
    int c;
    while((c = getc(stdin)) != EOF && c != '\n');
}

errors_e main_menu(void)
{
    errors_e rc = OK;
    action_e act = ACT_UNKNOWN;

    csc_matrix_t left_csc_matrix, right_csc_matrix;
    
    print_menu();
    printf("\nВведите число от 0 до 9: ");
    fscanf(stdin, "%u", &act);
    clear_stdin_buf();

    switch (act)
    {
        case ACT_EXIT:
            rc = EXIT;
            break;

        case ACT_PRINT_AUTOR_INFO:

            break;

        case ACT_READ_MATRIX:
            rc = read_matrix(&left_csc_matrix);
            if (rc == OK)
                rc = read_matrix(&right_csc_matrix);
            if (rc == OK)
                printf(GREEN "\nМатрицы успешно считаны!\n\n" RESET);
            break;
            
        case ACT_PRINT_MATRIX:
            puts("\nМатрица №1:\n");
            print_matrix(&left_csc_matrix);
            puts("Матрица №2:\n");
            print_matrix(&right_csc_matrix);
            printf(GREEN "\nМатрицы успешно выведены!\n\n" RESET);
            break;
        
        case ACT_PRINT_SUM_MATRIX:

            break;
        
        case ACT_PRINT_VECTORS:

            break;

        case ACT_SUM_MATRIX_STANDART:

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