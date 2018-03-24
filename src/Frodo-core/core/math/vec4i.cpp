#include "vec4.h"

namespace fd {
namespace core {
namespace math {

vec4i::vec4i() : x(0), y(0), z(0), w(0) {}

vec4i::vec4i(int32 x, int32 y, int32 z, int32 w) : x(x), y(y), z(z), w(w) {}

//int32
vec4i& vec4i::Add(const vec4i& v) {
	__m128i vxmm = _mm_set_epi32(v.w, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);
	w = M128I(xmm, 3);

	return *this;
}

//int32
vec4i& vec4i::Sub(const vec4i& v) {
	__m128i vxmm = _mm_set_epi32(v.w, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);
	w = M128I(xmm, 3);

	return *this;
}

//int32
vec4i& vec4i::Mul(const vec4i& v) {
	__m256i vxmm = _mm256_set_epi32(0, v.w, 0, v.z, 0, v.y, 0, v.x);
	__m256i xmm = _mm256_set_epi32(0, w, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, vxmm);

	x = M256I(xmm, 0);
	y = M256I(xmm, 2);
	z = M256I(xmm, 4);
	w = M256I(xmm, 6);

	return *this;
}

//int32
vec4i& vec4i::Div(const vec4i& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

//int32
vec4i& vec4i::Add(int32 v) {
	__m128i vxmm = _mm_set_epi32(v, v, v, v);
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);
	w = M128I(xmm, 3);

	return *this;
}

//int32
vec4i& vec4i::Sub(int32 v) {
	__m128i vxmm = _mm_set_epi32(v, v, v, v);
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = M128I(xmm, 0);
	y = M128I(xmm, 1);
	z = M128I(xmm, 2);
	w = M128I(xmm, 3);

	return *this;
}

//int32
vec4i& vec4i::Mul(int32 v) {
	__m256i vxmm = _mm256_set_epi32(0, v, 0, v, 0, v, 0, v);
	__m256i xmm = _mm256_set_epi32(0, w, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, vxmm);

	x = M256I(xmm, 0);
	y = M256I(xmm, 2);
	z = M256I(xmm, 4);
	w = M256I(xmm, 6);

	return *this;
}

//int32
vec4i& vec4i::Div(int32 v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;

	return *this;
}


}
}
}