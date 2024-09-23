#ifndef __ERRORS_H__
#define __ERRORS_H__

#define OK 0
#define ERR_IO 1
#define ERR_RANGE 2
#define ERR_BUF_OVERFLOW 3
#define ERR_EMPTY_STRING 4

void print_error(const int rc);

#endif //__ERRORS_H__