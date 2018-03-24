#include "vec4.h"

namespace fd {
namespace core {
namespace math {

vec4::vec4() : x(0), y(0), z(0), w(0) {}

vec4::vec4(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}


#pragma region vec-vec
//Asss
//float32
vec4& vec4::Add(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

//sub
//float32
vec4& vec4::Sub(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

//Mul
//float32
vec4& vec4::Mul(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

//Div
//float32
vec4& vec4::Div(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

#pragma endregion

#pragma region vec-shit

//Add
//float32
vec4& vec4::Add(float32 v) {
	__m128 vxmm = _mm_set_ps(v, v, v, v);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

//Sub
//float32
vec4& vec4::Sub(float32 v) {
	__m128 vxmm = _mm_set_ps(v, v, v, v);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

//Mul
//float32
vec4& vec4::Mul(float32 v) {
	__m128 vxmm = _mm_set_ps(v, v, v, v);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}

//Div
//float32
vec4& vec4::Div(float32 v) {
	__m128 vxmm = _mm_set_ps(v, v, v, v);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);
	w = M128(xmm, 3);

	return *this;
}


#pragma endregion

}
}
}