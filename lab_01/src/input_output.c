#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "input_output.h"

static void ent_int_num(void)
{
    printf("Введите целое число:\n");
    printf("1---5----10---15---20---25---30---35---40---45---50\n");
    printf("|   |    |    |    |    |    |    |    |    |    |\n");
}

static void ent_real_num(void)
{
    printf("Введите действительное число:\n");
    printf("1---5----10---15---20---25---30---35---40---45---50\n");
    printf("|   |    |    |    |    |    |    |    |    |    |\n");
}

static bool check_real_num(char *num)
{
    size_t digits = 0;

    //Проверка, что введено до 100 символов
    char *space = strchr(num, '\n');
    if (!space)
        return false;

    bool first_sign = false;   //Первый знак, опциональный (до експоненты)
    bool first_digit = false;  //Присутствие цифры после первого знака
    bool second_sign = false;  //Второй знак, опциональный (после експоненты)
    bool second_digit = false; //Присутствие цифры после второго знака
    bool exp_digit = false;
    bool digit = false;
    if (num[0] == '-' || num[0] == '+')
        first_sign = true;


    *space = '\0';
    if (strlen(num) <= first_sign)
        return false;

    bool dot = false;
    bool exp = false;

    size_t i = first_sign;
    while (i < strlen(num) && num[i] == '0')
    {
        i++;
        digit = true;
    }
    for (; i < strlen(num); ++i)
    {
        if (num[i] == '.')
        {
            if (exp || dot)
                return false;
            dot = true;
        }
        else if (num[i] == 'e' || num[i] == 'E')
        {
            if (exp || !first_digit)
                return false;
            num[i] = toupper(num[i]);
            exp = true;
        }
        else if (num[i] == '-' || num[i] == '+')
        {
            if (second_sign || !first_digit)
                return false;
            second_sign = true;
        }
        else if (isdigit(num[i]))
        {
            first_digit = true;
            digit = true;
            digits++;
            if (digits >= 41 && !exp)
                return false;
            if (second_sign)
                second_digit = true;
            if (exp)
                exp_digit = true;
        }
        else 
            return false;
    }

    if ((second_sign && !second_digit) || !digit || (exp && !exp_digit))
        return false;
        
    return true;
}

static bool check_int_num(char *num)
{
    size_t digits = 0;

    char *space = strchr(num, '\n');
    if (!space)
        return false;

    bool sign = false;
    if (num[0] == '-' || num[0] == '+')
        sign = true;

    *space = '\0';
    if (strlen(num) <= sign)
        return false;

    size_t i = 0;
    while (i < strlen(num) && num[i] == '0')
        i++;

    for (; i < strlen(num); ++i)
    {
        if (isdigit(num[i]))
            digits++;
        else
            return false;
        if (digits >= 41)
            return false;
    }    
    return true;
}

void print_start_info(FILE *file)
{
    fprintf(file, "\n\n  Автор: Палладий Евгений ИУ7-31Б. 16 вариант по списку, 4 вариант в ЛР\n\
  Программа выполняет деления целого числа до 40 значащих цифр на действительное число до 40 значащих цифр. Система счисления: 10\n\n\
   __________________________________________________________________________\n\
  | Правила ввода целого числа:                                              |\n\
  |                                                                          |\n\
  | 1) Количество значащих фицр: до 40                                       |\n\
  | 2) Допустим ввод ведущих нулей, они не считаются за значащие цифры       |\n\
  | 3) Опционален ввод +- перед числом  (+314) (-0031)                       |\n\
  |                                                                          |\n\
  | Как вводить запрещено:                                                   |\n\
  | 1) Пустой ввод: ( )                                                      |\n\
  | 2) Наличие посторонних символов: (0004t) (x) (1234d)                     |\n\
  | 3) Наличие двух знаков +- или неправильное его положение: (+-12) (37+3)  |\n\
  |__________________________________________________________________________|\n\n\
   __________________________________________________________________________\n\
  | Правила для правильного ввода вещественного числа:                       |\n\
  |                                                                          |\n\
  | 1) Количество значащих цифр: до 40                                       |\n\
  | 2) Допустим ввод ведущих нулей, они не считаются за значащие цифры       |\n\
  | 3) Размер порядка в отрезке [-99999; 99999]                              |\n\
  | 4) Допустимо отсутствие значащей цифры перед точкой: (.123 = 0.123E0)    |\n\
  | 5) Допустим ввод точки перед экспонентой: (12.e5 = 12e5 = 0.12E+7)       |\n\
  | 6) Допустимо отсутствие цифр после точки (12. = 12 = 0.12e2)             |\n\
  | 7) Опционален ввод +- перед числом или в экспоненте: (-31.43) (5.34e+6)  |\n\
  |                                                                          |\n\
  | Как вводить запрещено:                                                   |\n\
  | 1) Отсутствие значащих цифр: ( ) (.) (e4) (.e5)                          |\n\
  | 2) Отсутствие цифр в экспоненте: (123e) (e) (0.1e)                       |\n\
  | 3) Наличие посторонних символов: (12.k) (12e4t) (t.e5)                   |\n\
  | 4) Наличие двух точек или двух экспонент: (1.2.3) (12e4e5)               |\n\
  | 5) Наличие двух знаков +- или неправильное его положение: (+-12) (3+7.5) |\n\
  |__________________________________________________________________________|\n\n\
   _________________________________________________\n\
  | Ответом будет нормализованное число в формате:  |\n\
  |                                                 |\n\
  | [+-]?(0.m)E[+-]?(k)                             |\n\
  |                                                 |\n\
  | где:                                            |\n\
  |         1 <= m <= 40                            |\n\
  |         1 <= k <= 5                             |\n\
  | m - Количество цифр в мантиссе                  |\n\
  | k - Количество цифр в экспоненте                |\n\
  |_________________________________________________|\n\n\n");
}

int input_nums(char *str_int, char *str_real, FILE *file)
{
    char tmp1[100];
    ent_int_num();

    if (!fgets(tmp1, sizeof(tmp1), file))
        return ERR_IO;
    if (!check_int_num(tmp1))
        return ERR_INPUT_INT;  

    char tmp2[100];

    printf("\n");
    ent_real_num();

    if (!fgets(tmp2, sizeof(tmp2), file))
        return ERR_IO;
    if (!check_real_num(tmp2))
        return ERR_INPUT_REAL;  

    strcpy(str_int, tmp1);
    strcpy(str_real, tmp2);

    return OK;
}