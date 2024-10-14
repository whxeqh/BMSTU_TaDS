#include <stdio.h>
#include "UI.h"
#include "errors.h"

static void print_menu(void)
{
    fprintf(stdout,"    Выберите одно из слудующий действий:\
    0) Выход\n\
    1) Вывести информацию об авторе \n\
    2) Считать матрицы из файлы\n\
    3) Вывести матрицы в консоль \n\
    4) Вывести результат сложения матриц в консоль \n\
    5) Загрузить матрицу в файл\n\
    6) Вывести вектора (A, IA, JA) в консоль\n\
    7) Сложить две матрицы стандартным алгоритмом\n\
    8) Сложить две матрицы усовершенствованным алгоритмом\n\
    9) Произвести исследование\n\
    10) \n\
    11) \n\
    12) \n");
}

errors_e main_menu(void)
{
    errors_e rc = OK;
    action_e act;
    
    print_menu();
    fscanf(stdin, "%d", &act);

    switch (act)
    {
        case ACT_EXIT:
            rc = EXIT;
            break;

        case ACT_PRINT_AUTOR_INFO:

            break;

        case ACT_READ_MATRIX:

            break;
            
        case ACT_PRINT_MATRIX:

            break;
        
        case ACT_PRINT_SUM_MATRIX:

            break;
        
        case ACT_LOAD_MAXTRIX_IN_FILE:

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
            fprintf(stdout, "UNKNOWN ACT (%d)", act);
            break;
    }
}