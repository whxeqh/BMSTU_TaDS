#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "research.h"
#include "sort.h"
#include "UI.h"
#include "errors.h"
#include "country.h"
#include "countries_array.h"

// Функция подсчитывает время между двумя моментами в микросекундах
/*static unsigned long long calc_elapsed_time(const struct timespec *beg, const struct timespec *end)
{
    return ((unsigned long long)
        (end->tv_sec - beg->tv_sec) * 1000 * 1000 * 1000 +
                        (end->tv_nsec - beg->tv_nsec)) / 1000; 
}
*/

static bool get_num(int *num)
{
    char buf[10];
    fgets(buf, 10, stdin);
    char *p = strchr(buf, '\n');
    if (!p)
        return ERR_IO;
    *p = '\0';

    char *endtpr;
    int tmp = strtol(buf, &endtpr, 10);
    if (*endtpr != '\0' || (tmp < 0 || tmp > 12))
        return false;
    *num = tmp;
    return true;
}

static int get_file(FILE **file, const char *mode)
{
    char file_name[100];
    printf("Введите название файла: ");
    if (!fgets(file_name, sizeof(file_name), stdin))
        return ERR_IO;

    char *p = strchr(file_name, '\n');
    if (p)
        *p = '\0';
    else
        return ERR_IO;

    *file = fopen(file_name, mode);
    if (!*file)
        return ERR_FILE;
    
    return OK;
}

void clear_input_buffer(FILE *file)
{
    int c;
    do
    {
        c = fgetc(file);
    } while (isspace(c));
    ungetc(c, file);
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
        3) Сохранить список стран в файл\n\
        4) Добавить страну в конец списка\n\
        5) Удалить страну из списка по названию\n\
        6) Вывести список стран\n\
        7) Вывести список ключей\n\
        8) Вывести список стран по по списку ключей\n\
        9) Вывести список стран на выбранном материке, где можно заняться указанным видом спорта, со стоимостью отдыха меньше указанной\n\
        10) Отсортировать список ключей по столице\n\
        11) Отсортировать список фильмов по столице\n\
        12) Произвести и вывести исследование\n\
        0) Выход\n\n");
}

int select_from_menu(int *action)
{
    int tmp, rc = OK;

    printf("Ваш ввод: ");
    while(!get_num(&tmp))
    {
        printf("Повторите ввод: ");
        clear_input_buffer(stdin);
    }   
    
    printf("\n");

    *action = tmp;
    
    return rc;
}

int execute_action(const int action, country_t *countries, size_t *length, key_t *keys)
{

    //struct timespec t_beg_country, t_end_country;
    FILE *file = NULL;
    country_t country;
    int rc = OK;
    char field[MAX_COUNTRY_NAME_LENGTH];
    size_t pos;

    switch (action)
    {
        case ACT_EXIT:
            rc = EXIT;
            printf("ПОКА! \n\n");
            break;
        case ACT_PRINT_INFO:
            print_start_info();
            break;
        case ACT_LOAD_FROM_FILE:
            *length = 0;
            rc = get_file(&file, "r");
            if (rc != OK)
                return rc;
            do
            {
                rc = read_country(file, &country);
                clear_input_buffer(file);
                if (rc == OK)
                    rc = add_country_top(countries, country, length);
                //fprintf(file, "\n");
            } while (rc == OK && !feof(file));
    
            fclose(file);
            printf((rc == OK ? "\033[32m\nДанные успешно загружены\033[0m\n\n" : "\033[31mОшибка при загрузке\033[0m\n\n"));
            break;
        case ACT_PRINT_COUNTRIES:
            print_countries(stdout, countries, *length);
            printf("\033[32m\nВсе страны успешно выведены!\033[0m\n\n");
            break;
        case ACT_ADD_COUNTRY:
            memset(&country, 0, sizeof(country));
            rc = read_country(stdin, &country);
            if (rc == OK)
                rc = add_country_top(countries, country, length);
            if (rc == OK)
                printf("\033[32m\nСтрана успешно добавлена в конец списка!\033[0m\n\n");
            break;
        case ACT_SAVE_IN_FILE:
            rc = get_file(&file, "w");
            if (rc != OK)
                return rc;
            print_countries(file, countries, *length);
            printf("\033[32m\nДанные успешно записаны в файл!\033[0m\n\n");
            fclose(file);
            break;
        case ACT_DELETE_COUNTRY:
            fprintf(stdout, "Введите название страны: "); 
            rc = read_string(stdin, field, MAX_COUNTRY_NAME_LENGTH);
            if (rc != OK)
                break;
            rc = (find_in_array(countries, *length, field, &pos) ? OK : ERR_NOT_FOND);
            if (rc == OK)
            {
                delete_in_array(countries, length, pos);
                printf("\033[32m\nСтрана успешно удалена из списка!\033[0m\n\n");
            }
            break;
        case ACT_TASK:
            rc = print_task_countries(countries, *length);
            if (rc == OK)
                printf("\033[32m\nВсе подходящие страны успешно выведены!\033[0m\n\n");
            break;
        case ACT_PRINT_KEYS:
            print_keys(stdout, keys, *length);
            printf("\033[32m\nСписок ключей успешно выведен!\033[0m\n\n");
            break;
        case ACT_PRINT_COUNTRIES_BY_KEYS:
            print_countries_by_keys(countries, *length, keys);
            printf("\033[32m\nСписок стран по списку ключей успешно выведен!\033[0m\n\n");
            break;
        case ACT_SORT_KEYS:
            //clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_country);
            bubble_sort_keys(keys, (int) *length);
            //clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_country);
            //if (j > 1)
            //unsigned long long now = calc_elapsed_time(&t_beg_country, &t_end_country);
            //printf("DADADA: %lld\n", now);

            printf("\033[32m\nСписок стран по списку ключей успешно отсортирован по столицам!\033[0m\n\n");
            break;
        case ACT_SORT_COUNTRIES:
            //clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_country);
            bubble_sort_countries(countries, (int) *length);
            //clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_country);
            //if (j > 1)
            //now = calc_elapsed_time(&t_beg_country, &t_end_country);
            //printf("DADADA: %lld\n", now);

            //bubble_sort_countries(countries, (int) *length);
            printf("\033[32m\nСписок стран успешно отсортирован по столицам!\033[0m\n\n");
            break;
        case ACT_RESEARCH:
            rc = make_research();
            if (rc == OK)
                printf("\033[32m\nИследование проведено успешно! Все файлы отсортированы!\033[0m\n\n");
            else
                printf("\033[33m\nЧто-то пошло не так!(\033[0m\n\n");

            break;
        default:
            printf("\033[31mUNKNOWN_ACT (%d)\033[0m", action);
            return ERR_ACT;
    }

    if (action == ACT_DELETE_COUNTRY || action == ACT_ADD_COUNTRY || action == ACT_LOAD_FROM_FILE || action == ACT_SORT_COUNTRIES)
        fill_keys(countries, *length, keys);
    return rc;
}