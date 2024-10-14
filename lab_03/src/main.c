#include "errors.h"
#include "UI.h"
#include "matrix.h"

int main(void)
{
    errors_e rc = OK;
    
    do
    {
        rc = main_menu();
        if (rc != OK && rc != EXIT)
            print_error(rc);
    } while (rc != EXIT);
    
    return rc;
}