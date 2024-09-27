#include <string.h>
#include "countries_array.h"
#include "country.h"
#include "errors.h"
#include "UI.h"
#include "structures.h"

int main(void)
{
    //setbuf(stdout, NULL);
    int rc = OK, action;
    size_t length;
    country_t countries[MAX_LENGTH];
    key_t keys[MAX_LENGTH];
    
    do
    {
        print_menu();
        rc = select_from_menu(&action);
        if (rc == OK)
            rc = execute_action(action, countries, &length, keys);
        if (rc != OK && rc != EXIT)
            print_error(rc);
    } while (rc != EXIT);
    
    if (rc != OK && rc != EXIT)
        print_error(rc);

    return rc;
}