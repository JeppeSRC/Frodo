#include "vec4.h"

namespace fd {
namespace core {
namespace math {

vec4d::vec4d() : x(0), y(0), z(0), w(0) {}

vec4d::vec4d(float64 x, float64 y, float64 z, float64 w) : x(x), y(y), z(z), w(w) {}

//float64
vec4d& vec4d::Add(const vec4d& v) {
	__m256d vxmm = _mm256_set_pd(v.w, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_add_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Sub(const vec4d& v) {
	__m256d vxmm = _mm256_set_pd(v.w, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_sub_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Mul(const vec4d& v) {
	__m256d vxmm = _mm256_set_pd(v.w, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_mul_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Div(const vec4d& v) {
	__m256d vxmm = _mm256_set_pd(v.w, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_div_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Add(float64 v) {
	__m256d vxmm = _mm256_set_pd(v, v, v, v);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_add_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Sub(float64 v) {
	__m256d vxmm = _mm256_set_pd(v, v, v, v);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_sub_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Mul(float64 v) {
	__m256d vxmm = _mm256_set_pd(v, v, v, v);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_mul_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

//float64
vec4d& vec4d::Div(float64 v) {
	__m256d vxmm = _mm256_set_pd(v, v, v, v);
	__m256d xmm = _mm256_set_pd(w, z, y, x);
	xmm = _mm256_div_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	w = xmm.m256d_f64[3];

	return *this;
}

}
}
}