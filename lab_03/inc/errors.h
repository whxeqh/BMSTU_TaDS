#ifndef __ERRORS_H__
#define __ERRORS_H__

#define RESET "\033[0m"
#define RED "\033[31m"

typedef enum
{
    EXIT = -1,
    OK,
    ERR_IO,
    ERR_RANGE,
    ERR_FILE,
    ERR_MEMORY,
    ERR_ACT
} errors_e;

void print_error(errors_e rc);

#endif //__ERRORS_H__