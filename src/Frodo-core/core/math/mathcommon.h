#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <core/types.h>
#include <intrin.h>

#define FD_PRE_TO_RADIANS 0.01745329251994329576923690768489
#define FD_PRE_TO_DEGREES 57.295779513082320876798154814105

#define FD_TO_RADIANS_F(x) (x * (float32)FD_PRE_TO_RADIANS)
#define FD_TO_DEGREES_F(x) (x * (float32)FD_PRE_TO_DEGREES)

#define FD_PI M_PI

#define CEIL(x) ((int32)(x + 1))

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

#define FLOAT_CMP(a, b, type) (*((type*)&a) == *((type*)&b))

#define FLOAT32_CMP(a, b) FLOAT_CMP(a, b, uint32)
#define FLOAT64_CMP(a, b) FLOAT_CMP(a, b, uint64)