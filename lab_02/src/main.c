#include "country.h"
#include "errors.h"
#include "print.h"
#include "structures.h"

int main(void)
{
    int rc = OK;

    country_t country;

    rc = read_country(stdin, &country);

    if (rc != OK)
    {
        print_error(rc);
        return rc;
    }

    return rc;
}