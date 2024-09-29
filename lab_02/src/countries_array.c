#include <string.h>
#include <stdlib.h> 
#include <assert.h>
#include <ctype.h>
#include "errors.h"
#include "countries_array.h"
#include "country.h"
#include "structures.h"

/*
static void swap_countries(country_t *left, country_t *right)
{
    country_t tmp = *left;
    *left = *right;
    *right = tmp;
}*/

static void write_to_str(char *src, const size_t cur_pos, const char *ref)
{
    for (size_t i = 0; i < strlen(ref); ++i)
        src[i + cur_pos] = ref[i];
}

// Функция для центрирования строки в заданном буфере
static void print_centered_str(const char *str, const int max_buf)
{
    int str_len = strlen(str);
    int padding_left = (max_buf - str_len) / 2;
    int padding_right = max_buf - str_len - padding_left;
    
   fprintf(stdout, "%*s%s%*s|", padding_left, "", str, padding_right, "");
}

// Функция для центрирования числа в заданном буфере
void print_centered_number(const int num, const int max_buf)
{
    char str[max_buf];
    snprintf(str, sizeof(str), "%d", num);
    int str_len = strlen(str);
    int padding_left = (max_buf - str_len) / 2;
    int padding_right = max_buf - str_len - padding_left;

    fprintf(stdout, "%*s%s%*s|", padding_left, "", str, padding_right, "");
}

static void print_key(FILE *file_out, const key_t *key, const size_t id)
{
    if (file_out == stdout)
    {
        printf("            |");
        print_centered_number(id, 8);
        print_centered_number(key->ind, 30);
        print_centered_str(key->capital, MAX_CAPITAL_LENGTH);
        printf("\n");
    }
    else 
        fprintf(file_out, "%ld %ld %s\n", id, key->ind, key->capital);
}

static void print_country(FILE *file_out, const country_t *country)
{
    //char main_len[MAX_MAIN_TOURISM_LENGTH];
    if (file_out == stdout)
    {
        printf("        |");
        print_centered_str(country->name, MAX_COUNTRY_NAME_LENGTH);
        print_centered_str(country->capital, MAX_CAPITAL_LENGTH);
        print_centered_str(country->mainland, MAX_MAINLAND_LENGTH);
        print_centered_str((country->visa ? "+" : "-"), MAX_VISA_LENGTH);
        print_centered_number(country->flying_time, 14);
        print_centered_number(country->min_vacation_price, 23);

        char output_str[MAX_MAIN_TOURISM_LENGTH];
        char str[MAX_MAIN_TOURISM_LENGTH];
        memset(str, 0, sizeof(str));
        memset(output_str, 0, sizeof(output_str));
        switch (country->tourism)
        {
            case SIGHTSEEING:
                write_to_str(output_str, strlen(output_str), "Sightseeing: ");
                //Добавление кол-ва объектов к строке
    
                snprintf(str, sizeof(str), "%d", country->type.sightseeing.objects_amount);
                write_to_str(output_str, strlen(output_str), str);
                write_to_str(output_str, strlen(output_str), " - ");
                switch (country->type.sightseeing.objects_type)
                {
                    case NATURE:
                        write_to_str(output_str, strlen(output_str), "Nature");
                        break;
                    case ART:
                        write_to_str(output_str, strlen(output_str), "Art");
                        break;
                    case HISTORY:
                        write_to_str(output_str, strlen(output_str), "History");
                        break;
                    default:
                        fprintf(stderr, "\033[31mERR SIGHTSEEING OBJECT TYPE = \033[0m%d\n", country->type.sightseeing.objects_type);
                        exit(EXIT_FAILURE);
                }
                print_centered_str(output_str, sizeof(output_str));
                break;

            case BEACH:
                write_to_str(output_str, strlen(output_str), "Beach: ");
                write_to_str(output_str, strlen(output_str), country->type.beach.season);
                write_to_str(output_str, strlen(output_str), " - ");

                snprintf(str, sizeof(str), "%d", country->type.beach.air_temperature);
                write_to_str(output_str, strlen(output_str), str);
                write_to_str(output_str, strlen(output_str), " - ");

                snprintf(str, sizeof(str), "%d", country->type.beach.water_temperature);
                write_to_str(output_str, strlen(output_str), str);
                print_centered_str(output_str, sizeof(output_str));
                break;

            case SPORT:
                write_to_str(output_str, strlen(output_str), "Sport: ");
                switch (country->type.sport.sport_type)
                {
                    case MOUNTAIN_SKIING:
                        write_to_str(output_str, strlen(output_str), "Mountain skiing");
                        break;
                    case SURFING:
                        write_to_str(output_str, strlen(output_str), "Surfing");
                        break;
                    case CLIMBING:
                        write_to_str(output_str, strlen(output_str), "Climbing");
                        break;
                    default:
                        fprintf(stderr, "\033[31mERR SPORT TYPE = \033[0m%d\n", country->type.sport.sport_type);
                        exit(EXIT_FAILURE);
                }
                print_centered_str(output_str, sizeof(output_str));
                break;
            default:
                fprintf(stderr, "\033[31mERR COUNTRY TOURISM TYPE\033[0m\n");
                exit(EXIT_FAILURE);
        }
        //printf("\n        ________________________________________________________________________________________________________________________________________________\n");
        printf("\n\n");
    }
    else
    {
        fprintf(file_out, "%s %s %s %d %u %u", country->name, country->capital, country->mainland, country->visa, country->flying_time, country->min_vacation_price);
        switch (country->tourism)
        {
            case SIGHTSEEING:
                fprintf(file_out, " %d %u %u", SIGHTSEEING, country->type.sightseeing.objects_amount, country->type.sightseeing.objects_type);
                break;
            case BEACH:
                fprintf(file_out, " %d %s %d %d", BEACH, country->type.beach.season, country->type.beach.air_temperature, country->type.beach.water_temperature);
                break;
            case SPORT:
                fprintf(file_out, " %d %d\n", SPORT, country->type.sport.sport_type);
                break;
            default:
                fprintf(stderr, "\033[31mERR COUNTRY TOURISM TYPE\033[0m\n");
                exit(EXIT_FAILURE);
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


void delete_in_array(country_t *countries, size_t *length, const size_t pos)
{
    assert(*length > 0);
    for (size_t i = pos; i < *length - 1; ++i)
        countries[i] = countries[i + 1];
    (*length)--;
}

bool find_in_array(country_t *countries, const size_t length, char *field, size_t *pos)
{
    for (size_t i = 0; i < length; ++i)
    {
        //printf("%s : %s : %ld\n",countries[i].name, field, i);
        if (strcmp(countries[i].name, field) == 0)
        {
            *pos = i;
            return true;
        }
    }

    return false;
}

void fill_keys(country_t *countries, const size_t length, key_t *keys)
{
    for (size_t i = 0; i < length; ++i)
    {
        keys[i].ind = i;
        strcpy(keys[i].capital, countries[i].capital);
    }
}

int print_task_countries(const country_t countries[], const size_t length)
{
    int rc = OK;
    char mainland[MAX_MAINLAND_LENGTH];
    type_of_sport type;
    uint32_t min_vacation_price;

    //Ввод материка
    fprintf(stdout, "Введите название материка: ");
    rc = read_string(stdin, mainland, sizeof(mainland));
    if (rc != OK)
        return rc;

    //Ввод вида спорта
    fprintf(stdout, "Введите Вид спорта (1 - Горные лыжи, 2- Сёрфинг, 3 - Восхождения): ");    
    rc = read_sport_enum(stdin, &type);


    //Ввод стоимость отдыха
    fprintf(stdout, "Введите цену отдыха в долларах: ");
    rc = read_unsigned_num(stdin, &min_vacation_price);
    //clear_stdin();
    if (rc != OK)
        return rc;

    bool was = false;
    for (size_t i = 0; i < length; ++i)
    {
        bool same_mainland = (strcmp(mainland, countries[i].mainland) == 0 ? true : false);
        bool same_sport = (countries[i].type.sport.sport_type == type ? true : false);
        bool lower = (countries[i].min_vacation_price < min_vacation_price ? true : false);

        if (same_mainland && same_sport && lower)
        {
            if (!was)
                fprintf(stdout, \
        "\
         ________________________________________________________________________________________________________________________________________________\n\
        |   Название страны   |    Столица    |    Материк    |    Виза    | Время полета | Мин. стоимость отдыха |         Основной вид туризма         |\n\
        |________________________________________________________________________________________________________________________________________________|\n\n");
            print_country(stdout, &countries[i]);
            was = true;
        }
    }
    if (was)
        fprintf(stdout, "        |________________________________________________________________________________________________________________________________________________|\n\n");
    return (was ? OK : ERR_NOT_FOND);
}

void print_countries_by_keys(const country_t *countries, const size_t length, const key_t *keys)
{
    fprintf(stdout, \
        "\
         ________________________________________________________________________________________________________________________________________________\n\
        |   Название страны   |    Столица    |    Материк    |    Виза    | Время полета | Мин. стоимость отдыха |         Основной вид туризма         |\n\
        |________________________________________________________________________________________________________________________________________________|\n\n");
    
    for (size_t i = 0; i < length; ++i)
        print_country(stdout, &countries[keys[i].ind]);
            fprintf(stdout, "        |________________________________________________________________________________________________________________________________________________|\n\n");

}

void print_keys(FILE *file_out, key_t *keys, const size_t keys_length)
{
    if (file_out == stdout)
        fprintf(stdout, \
            "\
            _______________________________________________________\n\
            |   №   |    Индекс исходной таблицы    |    Столица    |\n\
            |_______________________________________________________|\n\n");


    for (size_t i = 0; i < keys_length; ++i)
        print_key(file_out, &keys[i], i);

    if (file_out == stdout)
        fprintf(stdout, "            |_______________________________________________________|\n\n");
}


void print_countries(FILE *file_out, const country_t countries[], const size_t length)
{
    if (file_out == stdout)
    {
        fprintf(file_out, \
        "\
         ________________________________________________________________________________________________________________________________________________\n\
        |   Название страны   |    Столица    |    Материк    |    Виза    | Время полета | Мин. стоимость отдыха |         Основной вид туризма         |\n\
        |________________________________________________________________________________________________________________________________________________|\n\n");
    }

    for (size_t i = 0; i < length; ++i)
    {
        print_country(file_out, &countries[i]);
        if (i != length - 1 || file_out != stdout)
            fprintf(file_out, "\n");
    }
    if (file_out == stdout)
        fprintf(file_out, "        |________________________________________________________________________________________________________________________________________________|\n\n");
}
