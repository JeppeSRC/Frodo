#include "vec3.h"

namespace fd {
namespace core {
namespace math {

vec3l::vec3l() : x(0), y(0), z(0) {}

vec3l::vec3l(int64 x, int64 y, int64 z) : x(x), y(y), z(z) {}

vec3l& vec3l::Add(const vec3l& v) {
	__m256i vxmm = _mm256_set_epi64x(0, v.z, v.y, v.x);
	__m256i xmm = _mm256_set_epi64x(0, z, y, x);
	xmm = _mm256_add_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);

	return *this;
}

vec3l& vec3l::Sub(const vec3l& v) {
	__m256i vxmm = _mm256_set_epi64x(0, v.z, v.y, v.x);
	__m256i xmm = _mm256_set_epi64x(0, z, y, x);
	xmm = _mm256_sub_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);

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
	__m256i vxmm = _mm256_set_epi64x(0, v, v, v);
	__m256i xmm = _mm256_set_epi64x(0, z, y, x);
	xmm = _mm256_add_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1); 
	z = M256L(xmm, 2);

	return *this;
}

//int64
vec3l& vec3l::Sub(int64 v) {
	__m256i vxmm = _mm256_set_epi64x(0, v, v, v);
	__m256i xmm = _mm256_set_epi64x(0, z, y, x);
	xmm = _mm256_sub_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);

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