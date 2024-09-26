#ifndef __INFO_H__
#define __INFO_H__

#include "structures.h"

typedef enum
{
    ACT_EXIT,
    ACT_PRINT_INFO,
    ACT_LOAD_FROM_FILE,
    ACT_SAVE_IN_FILE,
    ACT_ADD_COUNTRY,
    ACT_DELETE_COUNTRY,
    ACT_PRINT_COUNTRIES,
    ACT_PRINT_KEYS,
    ACT_PRINT_COUNTRIES_BY_KEYS,
    ACT_TASK,
    ACT_SORT_KEYS,
    ACT_SORT_COUNTRIES,
    ACT_UNKNOWN
} action_t;

void print_start_info(void);
void print_menu(void);
int select_from_menu(int *action);
int execute_action(const int action, country_t *countries, size_t *length, key_t *keys);

#endif //__PRINT_H__