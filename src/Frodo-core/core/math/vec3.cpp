#include "vec3.h"

namespace fd {
namespace core {
namespace math {

vec3::vec3() : x(0), y(0), z(0) {}

vec3::vec3(float32 x, float32 y, float32 z) : x(x), y(y), z(z) {}


#pragma region vec-vec
//Asss
//float32
vec3& vec3::Add(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

//sub
//float32
vec3& vec3::Sub(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

//Mul
//float32
vec3& vec3::Mul(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

//Div
//float32
vec3& vec3::Div(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

#pragma endregion

#pragma region vec-shit

//Add
//float32
vec3& vec3::Add(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

//Sub
//float32
vec3& vec3::Sub(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

//Mul
//float32
vec3& vec3::Mul(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}

//Div
//float32
vec3& vec3::Div(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];
	z = xmm.m128_f32[2];

	return *this;
}


#pragma endregion

}
}
}