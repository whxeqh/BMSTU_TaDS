#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "input_output.h"

#ifdef DEBUG
static void ent_int_num(void)
{
    printf("Введите целое число:\n");
    printf("1---5----10---15---20---25---30---35---40---45---50\n");
    printf("|   |    |    |    |    |    |    |    |    |    |\n");
}
#endif

#ifdef DEBUG
static void ent_real_num(void)
{
    printf("Введите действительное число:\n");
    printf("1---5----10---15---20---25---30---35---40---45---50\n");
    printf("|   |    |    |    |    |    |    |    |    |    |\n");
}
#endif

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
    if (num[0] == '-' || num[0] == '+')
        first_sign = true;


    *space = '\0';
    if (strlen(num) <= first_sign)
        return false;

    bool dot = false;
    bool exp = false;

    size_t i = first_sign;
    while (i < strlen(num) && num[i] == '0')
        i++;

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
            if (exp)
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
            digits++;
            if (digits >= 41 && !exp)
                return false;
            if (second_sign)
                second_digit = true;
        }
        else 
            return false;
    }

    if (second_sign && !second_digit)
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

#ifdef DEBUG
void print_start_info(FILE *file)
{
    fprintf(file, "\n\nАвтор: Палладий Евгений ИУ7-31Б. 16 вариант по списку, 4 вариант в ЛР\n \
Программа выполняет деления целого числа до 40 десятичных цифр на действительное число в форме \
+-m.n E +-K\nДлина мантиссы (m+n) - до 40 значащих цифр, а величина порядка K - до 5 цифр\n\n");
}
#endif

int input_nums(char *str_int, char *str_real, FILE *file)
{
    char tmp1[100];
    #ifdef DEBUG
    ent_int_num();
    #endif

    if (!fgets(tmp1, sizeof(tmp1), file))
        return ERR_IO;
    if (!check_int_num(tmp1))
        return ERR_INPUT_INT;  

    char tmp2[100];

    #ifdef DEBUG
    ent_real_num();
    #endif

    if (!fgets(tmp2, sizeof(tmp2), file))
        return ERR_IO;
    if (!check_real_num(tmp2))
        return ERR_INPUT_REAL;  

    strcpy(str_int, tmp1);
    strcpy(str_real, tmp2);

    return OK;
}