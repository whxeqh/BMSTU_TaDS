#include <string.h>
#include <stdlib.h>  // Для exit()
#include "errors.h"
#include "countries_array.h"
#include "country.h"
#include "structures.h"

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
                fprintf(file_out, "%d %d", SPORT, country->type.sport.sport_type);
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
