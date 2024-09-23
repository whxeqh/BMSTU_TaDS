#include "errors.h"
#include <stdio.h>

void print_error(const int rc)
{
    switch (rc)
    {
        case ERR_IO:
            fprintf(stderr, "INPUT/OTPUT ERROR!\n");
            break;
        case ERR_RANGE:
            fprintf(stderr, "RANGE ERROR!\n");
            break;
        case ERR_BUF_OVERFLOW:
            fprintf(stderr, "BUF OVERFLOW ERROR!\n");
            break;
        case ERR_EMPTY_STRING:
            fprintf(stderr, "EMPTY STRING ERROR!\n");
            break;
        default:
            fprintf(stderr, "UNKNOWN ERROR (%d)\n", rc);
            break;
    }
}