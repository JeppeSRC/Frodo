#include "vec2.h"

namespace fd {
namespace core {
namespace math {

vec2d::vec2d() : x(0), y(0) {}

vec2d::vec2d(float64 x, float64 y) : x(x), y(y) {}

//float64
vec2d& vec2d::Add(const vec2d& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_add_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Sub(const vec2d& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_sub_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Mul(const vec2d& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_mul_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Div(const vec2d& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_div_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Add(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_add_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Sub(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_sub_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Mul(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_mul_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//float64
vec2d& vec2d::Div(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_div_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

}}}