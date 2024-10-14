#ifndef __ERRORS_H__
#define __ERRORS_H__

typedef enum
{
    EXIT = -1,
    OK,
    ERR_IO,
    ERR_RANGE,
    ERR_FILE,
    ERR_MEMORY
} errors_e;

#endif //__ERRORS_H__