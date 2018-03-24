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

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

//sub
//float32
vec3& vec3::Sub(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

//Mul
//float32
vec3& vec3::Mul(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

//Div
//float32
vec3& vec3::Div(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

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

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

//Sub
//float32
vec3& vec3::Sub(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

//Mul
//float32
vec3& vec3::Mul(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

//Div
//float32
vec3& vec3::Div(float32 v) {
	__m128 vxmm = _mm_set_ps(0, v, v, v);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = M128(xmm, 0);
	y = M128(xmm, 1);
	z = M128(xmm, 2);

	return *this;
}

float32 vec3::Length() const {
	return sqrtf(x * x + y * y + z * z);
}

vec3& vec3::Normalize() {
	return Mul(1.0f / Length());
}

vec3 vec3::Cross(const vec3& v) const {
	__m128 xmm0 = _mm_set_ps(0, x, z, y);
	__m128 vxmm0 = _mm_set_ps(0, v.y, v.x, v.z);

	__m128 xmm1 = _mm_set_ps(0, y, x, z);
	__m128 vxmm1 = _mm_set_ps(0, v.x, v.z, v.y);

	xmm0 = _mm_mul_ps(xmm0, vxmm0);
	xmm1 = _mm_mul_ps(xmm1, vxmm1);

	xmm0 = _mm_sub_ps(xmm0, xmm1);

	return vec3(xmm0.m128_f32[0], xmm0.m128_f32[1], xmm0.m128_f32[2]);
}

float32 vec3::Dot(const vec3& v) const {
	__m128 xmm = _mm_set_ps(0, v.z, v.y, v.x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, y, z, x));
	return M128(xmm, 0) + M128(xmm, 1) + M128(xmm, 2);
}

#pragma endregion

}
}
}