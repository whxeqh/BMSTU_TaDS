#include "errors.h"
#include <stdio.h>

void print_error(const int rc)
{
    switch (rc)
    {
        case ERR_IO:
            fprintf(stderr, "\033[31mINPUT/OTPUT ERROR!\n\n\033[0m");
            break;
        case ERR_RANGE:
            fprintf(stderr, "\033[31mRANGE ERROR!\n\n\033[0m");
            break;
        case ERR_BUF_OVERFLOW:
            fprintf(stderr, "\033[31mBUF OVERFLOW ERROR!\033[0m\n\n");
            break;
        case ERR_EMPTY_STRING:
            fprintf(stderr, "\033[31mEMPTY STRING ERROR!\033[0m\n\n");
            break;
        case ERR_ACT:
            fprintf(stderr, "\033[31mACT ERROR\033[0m\n\n");
            break;
        case ERR_FILE:
            fprintf(stderr, "\033[31mERR WITH FILE\033[0m\n\n");
            break;
        case ERR_NOT_FOND:
            fprintf(stderr, "\033[31mERR COUNTRY NOT FOUND\033[0m\n\n");
            break;
        default:
            fprintf(stderr, "\033[31mUNKNOWN ERROR (%d)\n\n\033[0m", rc);
            break;
    }
}