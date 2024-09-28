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
    country_t countries[MAX_LENGTH];
    key_t keys[MAX_LENGTH];
    int length;

    char *file_name_data_countries = "research/data/countries/data.txt";
    FILE *file_data_countries = fopen(file_name_data_countries, "w"); 
    char *file_name_data_keys = "research/data/keys/data.txt";
    FILE *file_data_keys = fopen(file_name_data_keys, "w"); 

    char *file_name_flag_data_countries = "research/data/countries/flag_data.txt";
    FILE *file_flag_data_countries = fopen(file_name_flag_data_countries, "w"); 
    char *file_name_flag_data_keys = "research/data/keys/flag_data.txt";
    FILE *file_flag_data_keys = fopen(file_name_flag_data_keys, "w"); 

    int reps[] = {100, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000};
    
    for (size_t i = 0; rc == OK && i < sizeof(reps) / sizeof(reps[0]); ++i)
    {
        length = reps[i];
        unsigned long long total_time_countries = 0;
        unsigned long long total_time_keys = 0;
        unsigned long long total_time_flag_countries = 0;
        unsigned long long total_time_flag_keys = 0;

        //Выходной файл сортировки стран
        char file_name_countries_out[50];
        sprintf(file_name_countries_out, "research/data/countries/%u_out.txt", reps[i]);
        FILE *file_out_countries = fopen(file_name_countries_out, "w");

        //Выходной файл сортировки стран
        char file_name_countries_flag_out[50];
        sprintf(file_name_countries_flag_out, "research/data/countries/%u_flag_out.txt", reps[i]);
        FILE *file_out_countries_flag = fopen(file_name_countries_flag_out, "w");

        //Выходной файл сортировки ключей
        char file_name_keys_out[50];
        sprintf(file_name_keys_out, "research/data/keys/%u_out.txt", reps[i]);
        FILE *file_out_keys = fopen(file_name_keys_out, "w");

        //Выходной файл сортировки ключей
        char file_name_keys_flag_out[50];
        sprintf(file_name_keys_flag_out, "research/data/keys/%u_flag_out.txt", reps[i]);
        FILE *file_out_keys_flag = fopen(file_name_keys_flag_out, "w");

        //Запуск скрипта для генерации данных
        char script[100];
        sprintf(script, "python3 src/make_data.py %u_in.txt %u -r", reps[i], reps[i]);
        system(script);
        
        //Входной файл для работы со странами
        char file_name_countries_in[50];
        sprintf(file_name_countries_in, "research/data/countries/%u_in.txt", reps[i]);
        FILE *file_in_countries = fopen(file_name_countries_in, "r");

        //Входной файл сортировки ключей
        char file_name_keys_in[50];
        sprintf(file_name_keys_in, "research/data/keys/%u_in.txt", reps[i]);
        FILE *file_in_keys = fopen(file_name_keys_in, "w");
        for (size_t  j = 0; j < ITERATIONS; ++j)
        {
            rewind(file_in_countries);
            fill_countries(countries, file_in_countries);
            fill_keys(countries, length, keys);

            // Измерение времени обычной сортировки стран
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_country);
            bubble_sort_countries(countries, length);
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_country);
            //if (j > 1)
                total_time_countries += calc_elapsed_time(&t_beg_country, &t_end_country);
            //printf("BASE: %lld\n", calc_elapsed_time(&t_beg_country, &t_end_country));

            // Измерение времени обычной сортировки ключей
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_key);
            bubble_sort_keys(keys, length);
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_key);
            //if (j > 1)
                total_time_keys += calc_elapsed_time(&t_beg_key, &t_end_key);

            rewind(file_in_countries);
            fill_countries(countries, file_in_countries);
            fill_keys(countries, length, keys);
            
            // Измерение времени сортировки стран с флагом
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_country);
            flag_bubble_sort_countries(countries, length);
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_country);
            //printf("FLAG: %lld\n", calc_elapsed_time(&t_beg_country, &t_end_country));
            //if (j > 1)
                total_time_flag_countries += calc_elapsed_time(&t_beg_country, &t_end_country);

            // Измерение времени сортировки ключей с флагом
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg_key);
            flag_bubble_sort_keys(keys, length);
            clock_gettime(CLOCK_MONOTONIC_RAW, &t_end_key);
            //if (j > 1)
                total_time_flag_keys += calc_elapsed_time(&t_beg_key, &t_end_key);
        }
        
        print_countries(file_out_countries, countries, length);
        print_keys(file_out_keys, keys, length);
        print_countries(file_out_countries_flag, countries, length);
        print_keys(file_out_keys_flag, keys, length);
        print_keys(file_in_keys, keys, length);

        fclose(file_out_countries);
        fclose(file_out_keys);
        fclose(file_out_countries_flag);
        fclose(file_out_keys_flag);
        fclose(file_in_keys);
        fclose(file_in_countries);

        // Записываем среднее время для сортировки стран и ключей
        unsigned long long avg_time_countries = total_time_countries / ITERATIONS;
        unsigned long long avg_time_keys = total_time_keys / ITERATIONS;

        unsigned long long avg_time_flag_countries = total_time_flag_countries / ITERATIONS;
        unsigned long long avg_time_flag_keys = total_time_flag_keys / ITERATIONS;

        printf("Среднее время для %u элементов: страны = %llu, ключи = %llu\n", reps[i], avg_time_countries, avg_time_keys);
        printf("Среднее время с флагом для %u элементов: страны = %llu, ключи = %llu\n\n", reps[i], avg_time_flag_countries, avg_time_flag_keys);

        // Запись данных в файлы
        fprintf(file_data_countries, "%u %llu\n", reps[i], avg_time_countries);
        fprintf(file_data_keys, "%u %llu\n", reps[i], avg_time_keys);

        fprintf(file_flag_data_countries, "%u %llu\n", reps[i], avg_time_flag_countries);
        fprintf(file_flag_data_keys, "%u %llu\n", reps[i], avg_time_flag_keys);

        fflush(file_data_countries);
        fflush(file_data_keys);
        fflush(file_flag_data_countries);
        fflush(file_flag_data_keys);
    }

    fclose(file_data_countries);
    fclose(file_data_keys);
    fclose(file_flag_data_countries);
    fclose(file_flag_data_keys);
    return rc;
}
