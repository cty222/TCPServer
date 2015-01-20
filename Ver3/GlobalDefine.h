#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H

typedef int STATUS;
typedef unsigned char CHAR8;
typedef unsigned int UINT32;

#define IN
#define OUT

#define ERROR_NUM(x) (x|0x1000)
#define SUCCESS 0
#define ERR_READ_FAILDED ERROR_NUM(1)

#endif
