// common_types.h -- File added to facilitate utassert build.  In real project, project would
//                -- supply this file

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

// Standard integer types for portability
#include <stdint.h>

// Boolean type
typedef enum {
    FALSE = 0,
    TRUE = 1
	} boolean;

// Common data types
typedef unsigned char       uint8;
typedef signed char         int8t;
typedef unsigned short      uint16;
typedef signed short        int16;
typedef unsigned int        uint32;
typedef signed int          int32;
typedef unsigned long long  uint64;
typedef signed long long    int64;

// Macros for common operations or constants
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define PI 3.1415926535

#endif // COMMON_TYPES_H