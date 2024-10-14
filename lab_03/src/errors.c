#include "errors.h"
#include <stdio.h>

void print_error(errors_e rc)
{
    switch (rc)
    {
    case ERR_IO:
        puts("ERR WITH INPUT/OUTPUT\n");
        break;
    case ERR_RANGE:
        puts("ERR WITH RANGE\n");
        break;
    case ERR_FILE:
        puts("ERR WITH FILE\n");
        break;
    case ERR_MEMORY:
        puts("ERR WITH MEMORY\n");
        break;
    default:
        printf("UNKNOWN ERROR (%d)\n", rc);
        break;
    }
}

