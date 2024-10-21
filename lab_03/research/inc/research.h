#ifndef __RESEARCH_H__
#define __RESEARCH_H__

#include <stddef.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

#define REPS 15

void print_stats(const int *rows, const int *columns, const size_t *memory_standart, const size_t *memory_fast, const unsigned long long *times_standart, const unsigned long long *times_fast, const size_t length, const size_t percents);

void make_research(const int *rows, const int *columns, const size_t length, const int *persents, const size_t per_length);

#endif //__RESEARCH_H__
