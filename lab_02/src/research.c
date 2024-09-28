#define _POSIX_C_SOURCE 199309L
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

int make_research(void)
{   
    int rc = OK;
    srand(time(NULL));
    struct timespec t_beg, t_end;
    country_t countries[MAX_LENGTH], country;
    size_t length;

    char *file_name_data = "/research/data.txt";
    FILE *file_data = fopen(file_name_data, "w"); 

    unsigned int reps[] = {100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    
    for (size_t i = 0; i < sizeof(reps) / sizeof(reps[0]); ++i)
    {
        printf("DBG 38: reps = %u\n", reps[i]);
        char file_name_in[50], file_name_out[50];
        sprintf(file_name_in, "research/%u_in.txt", reps[i]);
        sprintf(file_name_out, "research/%u_out.txt", reps[i]);

        // Открытие файла для записи данных
        FILE *file_in = fopen(file_name_in, "w");
        FILE *file_out = fopen(file_name_out, "w");

        // Запуск скрипта для генерации данных
        char script[100];
        sprintf(script, "python3 src/make_data.py %u_in.txt %u -r", reps[i], reps[i]);
        system(script);

        length = reps[i];
        do
        {
            rc = read_country(file_in, &country);
            clear_input_buffer(file_in);
            if (rc == OK)
                rc = add_country_top(countries, country, &length);  // Передача указателя на length
        } while (rc == OK && !feof(file_in));

        fclose(file_in);
        fclose(file_out);

        // Измерение времени сортировки
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg);
        bubble_sort_countries(countries, length);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);

        // Подсчёт времени
        unsigned long long time = calc_elapsed_time(&t_beg, &t_end);

        fprintf(file_data, "%u %llu\n", reps[i], time);
        fflush(file_data);
    }

    fclose(file_data);
    return rc;
}
