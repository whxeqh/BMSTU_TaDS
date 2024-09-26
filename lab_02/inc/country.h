#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include <stdio.h>
#include <stddef.h>
#include "structures.h"

int read_country(FILE *file_in, country_t *country);
int read_string(FILE *file_in, char *string, const size_t max_buf_size);
int read_unsigned_num(FILE *file_in, uint32_t *flying_time);
int read_sport_enum(FILE *file_in, type_of_sport *type);
void clear_stdin(void);

#endif