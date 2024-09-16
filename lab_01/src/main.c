#include <stdio.h>
#include <string.h>
#include "big_nums.h"
#include "input_output.h"
#include "errors.h"
#include "division.h"

#define BUF_MAX 100

int main(void)
{
    int rc = OK;
    char str1[BUF_MAX];
    char str2[BUF_MAX];

    bdouble_t big_int, big_double, big_ans; 
    memset(&big_int, 0, sizeof(bdouble_t));
    memset(&big_double, 0, sizeof(bdouble_t));
    memset(&big_ans, 0, sizeof(bdouble_t));

    print_start_info(stdout);
    
    rc = input_nums(str1, str2, stdin);
    if (rc != OK)
    {
        print_error(rc, stderr);
        return rc;
    }

    //Парсинг числа из строки в структуру
    parce_numbers(str1, str2, &big_int, &big_double);

    printf("\nИсходные числа:\n");
    printf("-------------------------------------------------------------------------------\n");

    print_bdouble(&big_int);
    print_bdouble(&big_double);

    printf("-------------------------------------------------------------------------------\n\n");
    

    if (big_double.man_length == 1 && big_double.mantissa[0] == 0)
    {
        printf("ОШИБКА ДЕЛЕНИЯ НА 0\n");
        return 1;
    }

    big_ans = div_big_numbers(&big_int, &big_double);

    printf("Ответ:\n");
    printf("   1---5----10---15---20---25---30---35---40---45---50\n");
    printf("   |   |    |    |    |    |    |    |    |    |    |\n");
    print_bdouble(&big_ans);

    return OK;
}