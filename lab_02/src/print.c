#include <stdio.h>
#include "print.h"

void print_start_info(void)
{
    fprintf(stdout, "Автор: Палладий Евгений ИУ7-31Б. Вариант по списку: 16. Вариант по заданию: 2\n\n");
}

void print_menu(void)
{
    fprintf(stdout, "Выберите одно из действий (1-9):\n\
    1) Загрузить список стран из файла\n\
    2) Вывести список стран\n\
    3) Ввести страну\
    4) Удалить страну\n\n");
}