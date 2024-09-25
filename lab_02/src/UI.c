#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "UI.h"
#include "errors.h"
#include "country.h"
#include "countries_array.h"

static void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int get_file(FILE **file, const char *mode)
{
    char file_name[100];
    printf("Введите название файла: ");
    if (!fgets(file_name, sizeof(file_name), stdin))
        return ERR_IO;

    // Удаляем символ новой строки, если он есть
    file_name[strcspn(file_name, "\n")] = '\0';

    *file = fopen(file_name, mode);
    if (!*file)
        return ERR_FILE;
    
    return OK;
}

void print_start_info(void)
{
    fprintf(stdout, "Автор: Палладий Евгений ИУ7-31Б. Вариант по списку: 16. Вариант по заданию: 2\n\n");
}

void print_menu(void)
{
    fprintf(stdout, "Выберите одно из действий (1-11):\n\
        1) Вывести информацию о программе:\n\
        2) Загрузить список стран из файла\n\
        3) Вывести список стран\n\
        4) Добавить страну в конец массива\n\
        5) Удалить страну\n\
        6) Вывести список ключей\n\
        7) Сохранить массив в файл\n\
        8) \n\
        9) \n\
        10) \n\
        11) \n\
        12) \n\
        0) Выход\n\n");
}

int select_from_menu(int *action)
{
    int tmp, rc = OK;

    printf("Ваш ввод: ");
    char buf[10];
    fgets(buf, 10, stdin);
    tmp = atoi(buf);
    while (rc != EXIT && (tmp < 0 || tmp > 12))
    {
        printf("Ошибка ввода. Число должно быть от 0 до 12\n  Если хотите завершить программу, введите -1\n  Ваш ввод: ");
        scanf("%d", &tmp);
        if (tmp == -1 || tmp == 0)
            rc = EXIT;
    }

    if (rc != EXIT)
        *action = tmp;
    clear_input_buffer();
    
    return rc;
}

int execute_action(const int action, country_t *countries, size_t *length)
{
    FILE *file = NULL;
    country_t country;
    int rc = OK;

    switch (action)
    {
        case ACT_PRINT_INFO:
            print_start_info();
            break;
        case ACT_LOAD_FROM_FILE:
            break;
        case ACT_PRINT_COUNTRIES:
            print_countries(stdout, countries, *length);
            break;
        case ACT_ADD_COUNTRY:
            memset(&country, 0, sizeof(country));
            rc = read_country(stdin, &country);
            if (rc == OK)
                rc = add_country_top(countries, country, length);
            break;
        case ACT_PRINT_IN_FILE:
            rc = get_file(&file, "w");
            if (rc != OK)
                return rc;
            print_countries(file, countries, *length);
        case ACT_DELETE_COUNTRY:
            break;
        case ACT_PRINT_KEYS:
            break;
        default:
            printf("UNKNOWN_ACT (%d)", action);
            return ERR_ACT;
    }

    return rc;
}