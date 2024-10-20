#ifndef __RESEARCH_H__
#define __RESEARCH_H__

#include <stddef.h>

#define REPS 7

void print_stats(const int *rows, const int *times_standart, const int *times_fast, const size_t length, const size_t percents);

void make_research(const int *rows, const int *columns, const size_t length, const int *persents, const size_t per_length);

#endif //__RESEARCH_H__
