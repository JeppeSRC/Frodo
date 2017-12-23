#include "vec3.h"

namespace fd {
namespace core {
namespace math {

vec3l::vec3l() : x(0), y(0), z(0) {}

vec3l::vec3l(int64 x, int64 y, int64 z) : x(x), y(y), z(z) {}

vec3l& vec3l::Add(const vec3l& v) {
	__int64 a = v.x;
	__int64 b = v.y;
	__int64 c = v.z;
	__m256i vxmm = _mm256_set_epi64x(0, c, b, a);
	a = x;
	b = y;
	c = z;
	__m256i xmm = _mm256_set_epi64x(0, c, b, a);
	xmm = _mm256_add_epi64(xmm, vxmm);

	x = xmm.m256i_i64[0];
	y = xmm.m256i_i64[1];
	z = xmm.m256i_i64[2];

	return *this;
}

vec3l& vec3l::Sub(const vec3l& v) {
	__int64 a = v.x;
	__int64 b = v.y;
	__int64 c = v.z;
	__m256i vxmm = _mm256_set_epi64x(0, c, b, a);
	a = x;
	b = y;
	c = z;
	__m256i xmm = _mm256_set_epi64x(0, c, b, a);
	xmm = _mm256_sub_epi64(xmm, vxmm);

	x = xmm.m256i_i64[0];
	y = xmm.m256i_i64[1];
	z = xmm.m256i_i64[2];

	return *this;
}

//int64
vec3l& vec3l::Mul(const vec3l& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

vec3l& vec3l::Div(const vec3l& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

//int64
vec3l& vec3l::Add(int64 v) {
	__int64 a = v;
	__int64 b = v;
	__int64 c = v;
	__m256i vxmm = _mm256_set_epi64x(0, c, b, a);
	a = x;
	b = y;
	c = z;
	__m256i xmm = _mm256_set_epi64x(0, c, b, a);
	xmm = _mm256_add_epi64(xmm, vxmm);

	x = xmm.m256i_i64[0];
	y = xmm.m256i_i64[1];
	z = xmm.m256i_i64[2];

	return *this;
}

//int64
vec3l& vec3l::Sub(int64 v) {
	__int64 a = v;
	__int64 b = v;
	__int64 c = v;
	__m256i vxmm = _mm256_set_epi64x(0, c, b, a);
	a = x;
	b = y;
	c = z;
	__m256i xmm = _mm256_set_epi64x(0, c, b, a);
	xmm = _mm256_sub_epi64(xmm, vxmm);

	x = xmm.m256i_i64[0];
	y = xmm.m256i_i64[1];
	z = xmm.m256i_i64[2];

	return *this;
}

//int64
vec3l& vec3l::Mul(int64 v) {
	x *= v;
	y *= v;
	z *= v;

	return *this;
}

vec3l& vec3l::Div(int64 v) {
	x /= v;
	y /= v;
	z /= v;

	return *this;
}



}
}
}