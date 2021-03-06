#include "vec2.h"

namespace fd {
namespace core {
namespace math {

vec2l::vec2l() : x(0), y(0) {}

vec2l::vec2l(int64 x, int64 y) : x(x), y(y) {}

vec2l& vec2l::Add(const vec2l& v) {
	__m128i vxmm = _mm_set_epi64x(v.y, v.x);
	__m128i xmm = _mm_set_epi64x(y, x);
	xmm = _mm_add_epi64(xmm, vxmm);

	x = M128L(xmm, 0);
	y = M128L(xmm, 1);

	return *this;
}

vec2l& vec2l::Sub(const vec2l& v) {
	__m128i vxmm = _mm_set_epi64x(v.y, v.x);
	__m128i xmm = _mm_set_epi64x(y, x);
	xmm = _mm_sub_epi64(xmm, vxmm);

	x = M128L(xmm, 0);
	y = M128L(xmm, 1);

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
	__m128i vxmm = _mm_set_epi64x(v, v);
	__m128i xmm = _mm_set_epi64x(y, x);
	xmm = _mm_add_epi64(xmm, vxmm);

	x = M128L(xmm, 0);
	y = M128L(xmm, 1);

	return *this;
}

//int64
vec2l& vec2l::Sub(int64 v) {
	__m128i vxmm = _mm_set_epi64x(v, v);
	__m128i xmm = _mm_set_epi64x(y, x);
	xmm = _mm_sub_epi64(xmm, vxmm);

	x = M128L(xmm, 0);
	y = M128L(xmm, 1);

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