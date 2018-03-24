#include "vec3.h"

namespace fd {
namespace core {
namespace math {

vec3i::vec3i() : x(0), y(0), z(0) {}

vec3i::vec3i(int32 x, int32 y, int32 z) : x(x), y(y), z(z) {}

//int32
vec3i& vec3i::Add(const vec3i& v) {
	__m128i vxmm = _mm_set_epi32(0, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);

	return *this;
}

//int32
vec3i& vec3i::Sub(const vec3i& v) {
	__m128i vxmm = _mm_set_epi32(0, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);

	return *this;
}

//int32
vec3i& vec3i::Mul(const vec3i& v) {
	__m256i vxmm = _mm256_set_epi32(0, 0, 0, v.z, 0, v.y, 0, v.x);
	__m256i xmm = _mm256_set_epi32(0, 0, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, vxmm);

	x = M256I(xmm, 0);
	y = M256I(xmm, 2);
	z = M256I(xmm, 4);

	return *this;
}

//int32
vec3i& vec3i::Div(const vec3i& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

//int32
vec3i& vec3i::Add(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, v, v, v);
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);

	return *this;
}

//int32
vec3i& vec3i::Sub(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, v, v, v);
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);

	return *this;
}

//int32
vec3i& vec3i::Mul(int32 v) {
	__m256i vxmm = _mm256_set_epi32(0, 0, 0, v, 0, v, 0, v);
	__m256i xmm = _mm256_set_epi32(0, 0, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, vxmm);

	x = M256I(xmm, 0);
	y = M256I(xmm, 2);
	z = M256I(xmm, 4);

	return *this;
}

//int32
vec3i& vec3i::Div(int32 v) {
	x /= v;
	y /= v;
	z /= v;

	return *this;
}


}
}
}