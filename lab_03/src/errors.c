#include "errors.h"
#include <stdio.h>

void print_error(errors_e rc)
{
    switch (rc)
    {
    case ERR_IO:
        puts(RED "ERR WITH INPUT/OUTPUT\n" RESET);
        break;
    case ERR_RANGE:
        puts(RED "ERR WITH RANGE\n" RESET);
        break;
    case ERR_FILE:
        puts(RED "ERR WITH FILE\n" RESET);
        break;
    case ERR_MEMORY:
        puts(RED "ERR WITH MEMORY\n" RESET);
        break;
    case ERR_ACT:
        puts(RED "ERROR WITH CHOOSING ACT" RESET);
        break;
    default:
        printf(RED "UNKNOWN ERROR (%d)\n" RESET, rc);
        break;
    }
}

