#include "errors.h"
#include "UI.h"
#include "matrix.h"

int main(void)
{
    errors_e rc = OK;
    
    do
    {
        rc = main_menu();
    } while (rc != EXIT || rc != OK);
    
    return rc;
}