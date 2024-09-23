#include <string.h>
#include <stdlib.h>  // Для exit()
#include "errors.h"
#include "countries_array.h"
#include "country.h"
#include "structures.h"

// Функция для центрирования строки в заданном буфере
static void print_centered_str(const char *str, const int max_buf)
{
    int str_len = strlen(str);
    int padding_left = (max_buf - str_len) / 2;
    int padding_right = max_buf - str_len - padding_left;

    fprintf(stdout, "|%*s%s%*s|", padding_left, "", str, padding_right, "");
}

// Функция для центрирования числа в заданном буфере
void print_centered_number(const int num, const int max_buf)
{
    char str[max_buf];
    snprintf(str, sizeof(str), "%d", num);
    int str_len = strlen(str);
    int padding_left = (max_buf - str_len) / 2;
    int padding_right = max_buf - str_len - padding_left;

    fprintf(stdout, "|%*s%s%*s|", padding_left, "", str, padding_right, "");
}

static void print_country(FILE *file_out, const country_t country)
{
    if (file_out == stdout)
    {
        print_centered_str(country.name, MAX_COUNTRY_NAME_LENGTH);
        print_centered_str(country.capital, MAX_CAPITAL_LENGTH);
        print_centered_str(country.mainland, MAX_MAINLAND_LENGTH);
        print_centered_number(country.visa, 8);
        print_centered_number(country.flying_time, 14);
        print_centered_number(country.min_vacation_price, 23);

        char *str = NULL;

        switch (country.tourism)
        {
            case SIGHTSEEING:
                switch (country.type.sightseeing.objects_type)
                {
                    case NATURE:
                        str = "Природа";
                        break;
                    case ART:
                        str = "Искусство";
                        break;
                    case HISTORY:
                        str = "История";
                        break;
                    default:
                        fprintf(stderr, "ERR SIGHTSEEING OBJECT TYPE = %d\n", country.type.sightseeing.objects_type);
                        exit(EXIT_FAILURE);
                }
                fprintf(stdout, "| %d - %s |", country.type.sightseeing.objects_amount, str);
                break;

            case BEACH:
                fprintf(stdout, "| %s - %d - %d |", country.type.beach.season, country.type.beach.air_temperature, country.type.beach.water_temperature);
                break;

            case SPORT:
                switch (country.type.sport.sport_type)
                {
                    case MOUNTAIN_SKIING:
                        str = "Горные лыжи";
                        break;
                    case SURFING:
                        str = "Сёрфинг";
                        break;
                    case CLIMBING:
                        str = "Скалолазание";
                        break;
                    default:
                        fprintf(stderr, "ERR SPORT TYPE = %d\n", country.type.sport.sport_type);
                        exit(EXIT_FAILURE);
                }
                fprintf(stdout, "| %s |", str);
                break;
        }
    }
}

int add_country_top(country_t countries[], const country_t country, size_t *length)
{
    if (*length >= MAX_LENGTH)
        return ERR_RANGE;

    countries[*length] = country;
    (*length)++;

    return OK;
}

void print_countries(FILE *file_out, const country_t countries[], const size_t length)
{
    if (file_out == stdout)
    {
        fprintf(file_out, \
        " _________________________________________________________________________________________________________________________________\n\
        |   Название страны   |   Столица   |   Материк   |  Виза  | Время полета | Мин. стоимость отдыха |     Основной вид туризма     |\n\
        |________________________________________________________________________________________________________________________________|\n\n");
    }

    for (size_t i = 0; i < length; ++i)
        print_country(file_out, countries[i]);
}
