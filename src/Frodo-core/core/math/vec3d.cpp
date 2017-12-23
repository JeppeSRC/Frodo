#include "vec3.h"

namespace fd {
namespace core {
namespace math {

vec3d::vec3d() : x(0), y(0), z(0) {}

vec3d::vec3d(float64 x, float64 y, float64 z) : x(x), y(y), z(z) {}

//float64
vec3d& vec3d::Add(const vec3d& v) {
	__m256d vxmm = _mm256_set_pd(0, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_add_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];
	
	return *this;
}

//float64
vec3d& vec3d::Sub(const vec3d& v) {
	__m256d vxmm = _mm256_set_pd(0, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_sub_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

//float64
vec3d& vec3d::Mul(const vec3d& v) {
	__m256d vxmm = _mm256_set_pd(0, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_mul_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

//float64
vec3d& vec3d::Div(const vec3d& v) {
	__m256d vxmm = _mm256_set_pd(0, v.z, v.y, v.x);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_div_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

//float64
vec3d& vec3d::Add(float64 v) {
	__m256d vxmm = _mm256_set_pd(0, v, v, v);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_add_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

//float64
vec3d& vec3d::Sub(float64 v) {
	__m256d vxmm = _mm256_set_pd(0, v, v, v);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_sub_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

//float64
vec3d& vec3d::Mul(float64 v) {
	__m256d vxmm = _mm256_set_pd(0, v, v, v);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_mul_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

//float64
vec3d& vec3d::Div(float64 v) {
	__m256d vxmm = _mm256_set_pd(0, v, v, v);
	__m256d xmm = _mm256_set_pd(0, z, y, x);
	xmm = _mm256_div_pd(xmm, vxmm);

	x = xmm.m256d_f64[0];
	y = xmm.m256d_f64[1];
	z = xmm.m256d_f64[2];

	return *this;
}

}
}
}