#ifndef __UI_H__
#define __UI_H__

#include "errors.h"

#define RESET "\033[0m"
#define GREEN "\033[32m"

typedef enum 
{
    ACT_EXIT,
    ACT_PRINT_AUTOR_INFO,
    ACT_READ_MATRIX,
    ACT_PRINT_MATRIX,
    ACT_PRINT_SUM_MATRIX,
    ACT_PRINT_VECTORS,
    ACT_SUM_MATRIX_STANDART,
    ACT_SUM_MATRIX_FAST,
    ACT_MAKE_RESEARCH,
    ACT_UNKNOWN
} action_e;

void clear_stdin_buf(void);

errors_e main_menu(void);

#endif //__UI_H__