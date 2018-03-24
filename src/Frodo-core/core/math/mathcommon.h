#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <core/types.h>
#ifdef FD_WINDOWS
#include <intrin.h>

#define M128(x, i)  x.m128_f32[i]
#define M128D(x, i) x.m128d_f64[i]
#define M128I(x, i) x.m128i_i32[i]
#define M128L(x, i) x.m128i_i64[i]

#define M256D(x, i) x.m256d_f64[i]
#define M256I(x, i) x.m256i_i32[i]
#define M256L(x, i) x.m256i_i64[i]

#elif FD_LINUX
#include <x86intrin.h>

#define M128(x, i)  x[i]
#define M128D(x, i) M128(x, i)
#define M128I(x, i) (((int32*)&x)[i])
#define M128L(x, i) M128(x, i)

#define M256(x, i)  M128(x, i)
#define M256D(x, i) M256(x, i)
#define M256I(x, i) (((int32*)&x)[i])
#define M256L(x, i) M256(x, i)

#endif

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