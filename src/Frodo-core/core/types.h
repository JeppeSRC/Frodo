#pragma once
#include <stddef.h>

typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#ifdef FD_WINDOWS
typedef unsigned __int64 uint64;
#elif FD_LINUX
typedef unsigned long long uint64;
#endif

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

#ifdef FD_WINDOWS
typedef signed __int64 int64;
#elif FD_LINUX
typedef signed long long int64;
#endif

typedef float float32;
typedef double float64;

typedef size_t uint_t;