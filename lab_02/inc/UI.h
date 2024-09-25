#ifndef __INFO_H__
#define __INFO_H__

#include "structures.h"

typedef enum
{
    ACT_PRINT_INFO = 1,
    ACT_LOAD_FROM_FILE,
    ACT_PRINT_COUNTRIES,
    ACT_ADD_COUNTRY,
    ACT_DELETE_COUNTRY,
    ACT_PRINT_KEYS,
    ACT_PRINT_IN_FILE,
    //ACT_PRINT_INFO,
    //ACT_PRINT_INFO,
    //ACT_PRINT_INFO,
    //ACT_PRINT_INFO,
    ACT_UNKNOWN
} action_t;

void print_start_info(void);
void print_menu(void);
int select_from_menu(int *action);
int execute_action(const int action, country_t *countries, size_t *length);

#endif //__PRINT_H__