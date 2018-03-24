#include "vec2.h"

namespace fd {
namespace core {
namespace math {

vec2::vec2() : x(0), y(0) { }

vec2::vec2(float32 x, float32 y) : x(x), y(y) {}


#pragma region vec-vec
//Asss
//float32
vec2& vec2::Add(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	
	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

//sub
//float32
vec2& vec2::Sub(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

//Mul
//float32
vec2& vec2::Mul(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

//Div
//float32
vec2& vec2::Div(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

#pragma endregion

#pragma region vec-shit

//Add
//float32
vec2& vec2::Add(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

//Sub
//float32
vec2& vec2::Sub(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

//Mul
//float32
vec2& vec2::Mul(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}

//Div
//float32
vec2& vec2::Div(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);

	return *this;
}


#pragma endregion

}}}