#include <stdio.h>
#include "errors.h"

void print_error(int rc, FILE *file)
{
    switch (rc)
    {
        case ERR_IO:
            fprintf(file, "ОШИБКА ПРИ ВВОДЕ\n");
            break;
        case ERR_INPUT_INT:
            fprintf(file, "ОШИБКА ПРИ ВВОДЕ ЦЕЛОГО ЧИСЛА\n");
            break;
        case ERR_INPUT_REAL:
            fprintf(file, "ОШИБКА ПРИ ВВОДЕ ДЕЙСТВИТЕЛЬНОГО ЧИСЛА\n");
            break;
        case ERR_RANGE:
            fprintf(file, "ОШИБКА ДИАПАЗОНА\n");
            break;
        default:
            fprintf(file, "UNKNOWN ERROR (%d)\n", rc);
            break;
    }
}