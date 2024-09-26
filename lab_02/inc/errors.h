#ifndef __ERRORS_H__
#define __ERRORS_H__

#define EXIT -1
#define OK 0
#define ERR_IO 1
#define ERR_RANGE 2
#define ERR_BUF_OVERFLOW 3
#define ERR_EMPTY_STRING 4
#define ERR_ACT 5
#define ERR_FILE 6
#define ERR_NOT_FOND 7

void print_error(const int rc);

#endif //__ERRORS_H__