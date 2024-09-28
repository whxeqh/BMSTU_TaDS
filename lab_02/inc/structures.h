#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_COUNTRY_NAME_LENGTH (20 + 1)
#define MAX_MAINLAND_LENGTH (14 + 1)
#define MAX_CAPITAL_LENGTH (14 + 1)
#define MAX_SEASON_LENGTH (10 + 1)
#define MAX_VISA_LENGTH (11 + 1)
#define MAX_SPORT_LENGTH (15 + 1)
#define MAX_MAIN_TOURISM_LENGTH (37 + 1)

//Вид туризма
typedef enum 
{
    SIGHTSEEING = 1,         // Экскурсионный
    BEACH,              // Спортивный
    SPORT                // Пляжный 
} type_of_tourism;

//Вид объектов
typedef enum 
{
    NATURE = 1,           // История
    ART,              // Природа
    HISTORY                  // Искусство 
} type_of_objects;

//Вид спорта
typedef enum 
{
    MOUNTAIN_SKIING = 1,     // Природа
    SURFING,
    CLIMBING            // История              // Искусство 
} type_of_sport;

// Экскурсионный вид отдыха
typedef struct
{
    uint32_t objects_amount;               // Количество объектов
    type_of_objects objects_type;                  // Вид объекта
} sightseeing_t;   


// Пляжный вид отдыха
typedef struct
{
    char season[MAX_SEASON_LENGTH];            // Сезон
    short water_temperature;                   // Темеература воды
    short air_temperature;                     // Температура воздуха
} beach_t;

// Спорттивный вид отдыха
typedef struct
{
    type_of_sport sport_type;
} sport_t;

// Объединение видов туризма
typedef union 
{
    sightseeing_t sightseeing;              // Экскурсионный
    beach_t beach;                          // Пляжный
    sport_t sport;                          // Спортивный
} type_t;
 

// Структура фильма
typedef struct 
{
    char name[MAX_COUNTRY_NAME_LENGTH];
    char capital[MAX_CAPITAL_LENGTH];       // Название столицы
    char mainland[MAX_MAINLAND_LENGTH];     // Название материка
    bool visa;                              // Потребность в визу
    uint32_t flying_time;                   // Время полета в минутах
    uint32_t min_vacation_price;            // Минимальная цена отдыха в долларах
    type_of_tourism tourism;                // Перечисление туризма
    type_t type;                            // Union со структурой туризма
} country_t;

typedef struct
{
    size_t ind;
    char capital[MAX_CAPITAL_LENGTH];
} key_t;


#endif // __STRUCTURES_H__
