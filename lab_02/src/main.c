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
    size_t length = 0;
    country_t countries[MAX_LENGTH];
    memset(countries, 0, sizeof(countries));
    
    do
    {
        print_menu();
        rc = select_from_menu(&action);
        if (rc == EXIT)
            break;
        //int c;
        //while ((c = getchar()) != '\n' && c != EOF);
        if (rc == OK)
            rc = execute_action(action, countries, &length);
    } while (rc == OK);
    
    if (rc != OK && rc != EXIT)
        print_error(rc);

    return rc;
}