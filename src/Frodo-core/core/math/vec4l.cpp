#include "vec4.h"

namespace fd {
namespace core {
namespace math {

vec4l::vec4l() : x(0), y(0), z(0), w(0) {}

vec4l::vec4l(int64 x, int64 y, int64 z, int64 w) : x(x), y(y), z(z), w(w) {}

vec4l& vec4l::Add(const vec4l& v) {
	__m256i vxmm = _mm256_set_epi64x(v.w, v.z, v.y, v.x);
	__m256i xmm = _mm256_set_epi64x(w, z, y, x);
	xmm = _mm256_add_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);
	w = M256L(xmm, 3);

	return *this;
}

vec4l& vec4l::Sub(const vec4l& v) {
	__m256i vxmm = _mm256_set_epi64x(v.w, v.z, v.y, v.x);
	__m256i xmm = _mm256_set_epi64x(w, z, y, x);
	xmm = _mm256_sub_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);
	w = M256L(xmm, 3);

	return *this;
}

//int64
vec4l& vec4l::Mul(const vec4l& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

vec4l& vec4l::Div(const vec4l& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

//int64
vec4l& vec4l::Add(int64 v) {
	__m256i vxmm = _mm256_set_epi64x(v, v, v, v);
	__m256i xmm = _mm256_set_epi64x(w, z, y, x);
	xmm = _mm256_add_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);
	w = M256L(xmm, 3);

	return *this;
}

//int64
vec4l& vec4l::Sub(int64 v) {
	__m256i vxmm = _mm256_set_epi64x(v, v, v, v);
	__m256i xmm = _mm256_set_epi64x(w, z, y, x);
	xmm = _mm256_sub_epi64(xmm, vxmm);

	x = M256L(xmm, 0);
	y = M256L(xmm, 1);
	z = M256L(xmm, 2);
	w = M256L(xmm, 3);

	return *this;
}

//int64
vec4l& vec4l::Mul(int64 v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;

	return *this;
}

vec4l& vec4l::Div(int64 v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;

	return *this;
}



}
}
}