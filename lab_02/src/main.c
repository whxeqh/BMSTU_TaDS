#include <string.h>
#include "countries_array.h"
#include "country.h"
#include "errors.h"
#include "UI.h"
#include "structures.h"

int main(void)
{
    setbuf(stdout, NULL);
    int rc = OK, action;
    do
    {
        print_menu();
        rc = select_from_menu(&action);
        if (rc == EXIT)
            break;
        rc = execute_action(action);
    } while (rc == OK);
    
    if (rc != OK && rc != EXIT)
        print_error(rc);

    return rc;
}