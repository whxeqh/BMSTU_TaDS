#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "country.h"
#include "errors.h"

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


static int read_string(FILE *file_in, char *string, const size_t max_buf_size)
{
    if (file_in == stdin)
    {
        if (!fgets(string, max_buf_size, file_in))
            return ERR_IO;
        
        char *p = strchr(string, '\n');
        if (!p)
        {
            clear_stdin();
            return ERR_BUF_OVERFLOW;
        }
        *p = '\0';

        if (strlen(string) == 0)
            return ERR_EMPTY_STRING;
    }
    else 
    {
        char word[100];
         if (fscanf(file_in, "%99s", word) != 1)
            return ERR_IO; 

        if (strlen(word) >= max_buf_size)
            return ERR_BUF_OVERFLOW;

        strcpy(string, word);

        if (strlen(string) == 0)
            return ERR_EMPTY_STRING;
    }
    return OK;
}

static int read_bool(FILE *file_in, bool *visa)
{
    int tmp;
    if (!fscanf(file_in, "%d", &tmp))
        return ERR_IO;
    
    if (file_in == stdin)
        clear_stdin();

    if (tmp != 0 && tmp != 1)
        return ERR_RANGE; 
        
    *visa = tmp;
    
    return OK;
}

static int read_unsigned_num(FILE *file_in, uint32_t *flying_time)
{
    int tmp;
    if (!fscanf(file_in, "%d", &tmp))
        return ERR_IO;
    
    if (file_in == stdin)
        clear_stdin();
    
    if (tmp < 0)
        return ERR_RANGE; 
    
    *flying_time = tmp;
    
    return OK;
}

static int read_short_num(FILE *file_in, short *flying_time)
{
    int tmp;
    if (!fscanf(file_in, "%d", &tmp))
        return ERR_IO;
    
    if (file_in == stdin)
        clear_stdin();

    if (tmp < SHRT_MIN || tmp > SHRT_MAX)
        return ERR_RANGE; 
    
    *flying_time = tmp;
    
    return OK;
}

static int read_tourism_enum(FILE *file_in, type_of_tourism *type)
{
    int tmp;
    if (!fscanf(file_in, "%d", &tmp))
        return ERR_IO;

    if (tmp < 1 || tmp > 3)
        return ERR_RANGE;

    *type = tmp;
    if (file_in == stdin)
        clear_stdin();

    return OK;
}

static int read_object_enum(FILE *file_in, type_of_objects *type)
{
    int tmp;
    if (!fscanf(file_in, "%d", &tmp))
        return ERR_IO;

    if (file_in == stdin)
        clear_stdin();

    if (tmp < 1 || tmp > 3)
        return ERR_RANGE;

    *type = tmp;

    return OK;
}

static int read_sport_enum(FILE *file_in, type_of_sport *type)
{
    int tmp;
    if (!fscanf(file_in, "%d", &tmp))
        return ERR_IO;

    if (file_in == stdin)
        clear_stdin();

    if (tmp < 1 || tmp > 3) 
        return ERR_RANGE;

    *type = tmp;
    
    return OK;
}

static int read_sightseeing_t(FILE *file_in, sightseeing_t *ref)
{
    int rc = OK;
    sightseeing_t tmp;
    memset(&tmp, 0, sizeof(sightseeing_t));

    if (file_in == stdin)
        fprintf(stdout, "Введите количество объектов: ");
    rc = read_unsigned_num(file_in, &tmp.objects_amount);
    if (rc != OK)
        return rc;    

    if (file_in == stdin)
        fprintf(stdout, "Введите основной вид объектов (1 - природа, 2 - искусство, 3 - история): ");    
    rc = read_object_enum(file_in, &tmp.objects_type);

    if (rc == OK)
        *ref = tmp;
    return rc;
}

static int read_beach_t(FILE *file_in, beach_t *ref)
{
    int rc = OK;
    beach_t tmp;
    memset(&tmp, 0, sizeof(beach_t));

    if (file_in == stdin)
        fprintf(stdout, "Введите основной сезон: ");
    rc = read_string(file_in, tmp.season, sizeof(tmp.season));
    if (rc != OK)
        return rc;    

    if (file_in == stdin)
        fprintf(stdout, "Введите температуру воздуха: ");
    rc = read_short_num(file_in, &tmp.air_temperature);
    if (rc != OK)
        return rc; 

    if (file_in == stdin)
        fprintf(stdout, "Введите температуру воды: ");
    rc = read_short_num(file_in, &tmp.water_temperature);
    if (rc != OK)
        return rc;   

    if (rc == OK)
        *ref = tmp;
    return rc;
}

static int read_sport_t(FILE *file_in, sport_t *sport)
{
    int rc = OK;
    sport_t tmp;
    memset(&tmp, 0, sizeof(sport_t));

    if (file_in == stdin)
        fprintf(stdout, "Введите Вид спорта (1 - Горные лыжи, 2- Сёрфинг, 3 - Восхождения): ");    
    rc = read_sport_enum(file_in, &tmp.sport_type);

    *sport = tmp;
    return rc;
}



int read_country(FILE *file_in, country_t *country)
{
    int rc = OK;
    country_t tmp;
    memset(&tmp, 0, sizeof(country_t));

    if (file_in == stdin)
        fprintf(stdout, "Введите название страны: ");
    rc = read_string(file_in, tmp.name, sizeof(tmp.name)); 
    if (rc != OK)
        return rc;

    if (file_in == stdin)
        fprintf(stdout, "Введите название столицы: ");    
    rc = read_string(file_in, tmp.capital, sizeof(tmp.capital));      
    if (rc != OK)
        return rc;

    if (file_in == stdin)
        fprintf(stdout, "Введите название материка: ");
    rc = read_string(file_in, tmp.mainland, sizeof(tmp.mainland));  
    if (rc != OK)
        return rc;  

    if (file_in == stdin)
        fprintf(stdout, "Введите требуемость визы (0 - не нужна, 1 - нужна): ");
    rc = read_bool(file_in, &tmp.visa);                              
    if (rc != OK)
        return rc;
    
    if (file_in == stdin)
        fprintf(stdout, "Введите время полета в минутах: ");
    rc = read_unsigned_num(file_in, &tmp.flying_time);         
    if (rc != OK)
        return rc;

    if (file_in == stdin)
        fprintf(stdout, "Введите минимальную цену отдыха в долларах: ");
    rc = read_unsigned_num(file_in, &tmp.min_vacation_price); 
    if (rc != OK)
        return rc;
    
    if (file_in == stdin)
        fprintf(stdout, "Введите основной вид туризма (1 - Экскурсионный, 2 - Пляжный, 3 - Спортивный): ");
    rc = read_tourism_enum(file_in, &tmp.tourism); 
    if (rc != OK)
        return rc;        
    
    switch (tmp.tourism)
    {
        case SIGHTSEEING:
            rc = read_sightseeing_t(file_in, &tmp.type.sightseeing);
            break;
        case BEACH:
            rc = read_beach_t(file_in, &tmp.type.beach);
            break;
        case SPORT:
            rc = read_sport_t(file_in, &tmp.type.sport);
            break;
        default:
            rc = ERR_RANGE;
            break;
    }

    if (rc == OK)
        *country = tmp;

    return rc;

}