#define _POSIX_C_SOURCE 199309L
#include "structures.h"
#include "research.h"
#include "countries_array.h"
#include "sort.h"
#include "country.h"
#include "errors.h"
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Функция подсчитывает время между двумя моментами в микросекундах
static unsigned long long calc_elapsed_time(const struct timespec *beg, const struct timespec *end)
{
    return ((unsigned long long)
        (end->tv_sec - beg->tv_sec) * 1000 * 1000 * 1000 +
                        (end->tv_nsec - beg->tv_nsec)) / 1000; 
}

static int fill_countries(country_t *countries, FILE *file_in)
{
    int rc = OK;
    country_t country;
    size_t length = 0;

    do
    {
        rc = read_country(file_in, &country);
        clear_input_buffer(file_in);
        if (rc == OK)
            rc = add_country_top(countries, country, &length);  // Передача указателя на length
    } while (rc == OK && !feof(file_in));
    return rc;
}

int make_research(void)
{   
    int rc = OK;
    srand(time(NULL));
    struct timespec t_beg_country, t_beg_key, t_end_country, t_end_key;
    country_t countries[MAX_LENGTH], country;
    key_t keys[MAX_LENGTH];
    //printf("%d\n", sizeof(countries));
    size_t length;

    char *file_name_data_countries = "research/data/countries/data.txt";
    FILE *file_data_countries = fopen(file_name_data_countries, "w"); 
    char *file_name_data_keys = "research/data/keys/data.txt";
    FILE *file_data_keys = fopen(file_name_data_keys, "w"); 

    unsigned int reps[] = {100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000};
    
    for (size_t i = 0; rc == OK && i < sizeof(reps) / sizeof(reps[0]); ++i)
    {
        length = reps[i];
        memset(&country, 0, sizeof(country));
        memset(countries, 0, sizeof(countries));
        memset(keys, 0, sizeof(keys));

        // Запуск скрипта для генерации данных
        char script[100];
        sprintf(script, "python3 src/make_data.py %u_in.txt %u -r", reps[i], reps[i]);
        system(script);


        printf("Обрабатывается файл %u.txt\n", reps[i]);

        //Файлы для работы со странами
        char file_name_countries_in[50], file_name_countries_out[50];
        sprintf(file_name_countries_in, "research/data/countries/%u_in.txt", reps[i]);
        sprintf(file_name_countries_out, "research/data/countries/%u_out.txt", reps[i]);
        FILE *file_in_countries = fopen(file_name_countries_in, "r");
        FILE *file_out_countries = fopen(file_name_countries_out, "w");

        printf("DBG 79: file_in_countries = %s\n", file_name_countries_in);

        //Файлы для работы с ключами
        char file_name_keys_in[50], file_name_keys_out[50];
        sprintf(file_name_keys_in, "research/data/keys/%u_in.txt", reps[i]);
        sprintf(file_name_keys_out, "research/data/keys/%u_out.txt", reps[i]);
        FILE *file_in_keys = fopen(file_name_keys_in, "r");
        FILE *file_out_keys = fopen(file_name_keys_out, "w");

        rc = fill_countries(countries, file_in_countries);
        fclose(file_in_countries);
        if (rc != OK)
            return rc;

        fill_keys(countries, reps[i], keys);

        // Измерение времени сортировки стран
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_country);
        bubble_sort_countries(countries, length);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_country);
        unsigned long long time_countries = calc_elapsed_time(&t_beg_country, &t_beg_country);

        //Печать отсортированного массива стран в файл
        print_countries(file_out_countries, countries, length);
        fclose(file_out_countries);


        print_keys(file_in_keys, keys, length);
        fclose(file_in_keys);
        // Измерение времени сортировки ключей
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_key);
        bubble_sort_keys(keys, length);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_key);
        unsigned long long time_keys = calc_elapsed_time(&t_beg_key, &t_beg_key);

        //Печать отсортированного массива ключей в файл
        print_keys(file_out_keys, keys, length);
        fclose(file_out_keys);

        fprintf(file_data_countries, "%u %llu\n", reps[i], time_countries);fflush(file_data_countries);
        fflush(file_data_countries);
        fprintf(file_data_keys, "%u %llu\n", reps[i], time_keys);
        fflush(file_data_keys);
    }

    fclose(file_data_countries);
    fclose(file_data_keys);
    return rc;
}
