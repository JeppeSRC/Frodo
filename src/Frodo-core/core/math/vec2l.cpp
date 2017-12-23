#include "vec2.h"

namespace fd {
namespace core {
namespace math {

vec2l::vec2l() : x(0), y(0) {}

vec2l::vec2l(int64 x, int64 y) : x(x), y(y) {}

vec2l& vec2l::Add(const vec2l& v) {
	__int64 a = v.x;
	__int64 b = v.y;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_add_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

vec2l& vec2l::Sub(const vec2l& v) {
	__int64 a = v.x;
	__int64 b = v.y;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_sub_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//int64
vec2l& vec2l::Mul(const vec2l& v) {
	x *= v.x;
	y *= v.y;

	return *this;
}

vec2l& vec2l::Div(const vec2l& v) {
	x /= v.x;
	y /= v.y;

	return *this;
}

//int64
vec2l& vec2l::Add(int64 v) {
	__int64 a = v;
	__int64 b = v;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_add_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//int64
vec2l& vec2l::Sub(int64 v) {
	__int64 a = v;
	__int64 b = v;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_sub_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//int64
vec2l& vec2l::Mul(int64 v) {
	x *= v;
	y *= v;

	return *this;
}

vec2l& vec2l::Div(int64 v) {
	x /= v;
	y /= v;

	return *this;
}



}
}
}