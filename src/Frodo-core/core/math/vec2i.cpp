#include "vec2.h"

namespace fd {
namespace core {
namespace math {

vec2i::vec2i() : x(0), y(0) {}

vec2i::vec2i(int32 x, int32 y) : x(x), y(y) {}

//int32
vec2i& vec2i::Add(const vec2i& v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

//int32
vec2i& vec2i::Sub(const vec2i& v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

//int32
vec2i& vec2i::Mul(const vec2i& v) {
	__m128i vxmm = _mm_set_epi32(0, v.y, 0, v.x);
	__m128i xmm = _mm_set_epi32(0, y, 0, x);
	xmm = _mm_mul_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[2];

	return *this;
}

//int32
vec2i& vec2i::Div(const vec2i& v) {
	x /= v.x;
	y /= v.y;
	return *this;
}

//int32
vec2i& vec2i::Add(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v, v);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

//int32
vec2i& vec2i::Sub(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v, v);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

//int32
vec2i& vec2i::Mul(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, v, 0, v);
	__m128i xmm = _mm_set_epi32(0, y, 0, x);
	xmm = _mm_mul_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[2];

	return *this;
}

//int32
vec2i& vec2i::Div(int32 v) {
	x /= v;
	y /= v;

	return *this;
}


}
}
}