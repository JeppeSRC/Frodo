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

float64 vec3d::Length() const {
	return sqrt(x * x + y * y + z * z);
}

vec3d& vec3d::Normalize() {
	return Mul(1.0 / Length());
}

vec3d vec3d::Cross(const vec3d& v) const {
	__m256d xmm0 = _mm256_set_pd(0, x, z, y);
	__m256d vxmm0 = _mm256_set_pd(0, v.y, v.x, v.z);

	__m256d xmm1 = _mm256_set_pd(0, y, x, z);
	__m256d vxmm1 = _mm256_set_pd(0, v.x, v.z, v.y);

	xmm0 = _mm256_mul_pd(xmm0, vxmm0);
	xmm1 = _mm256_mul_pd(xmm1, vxmm1);

	xmm0 = _mm256_sub_pd(xmm0, xmm1);

	return vec3d(xmm0.m256d_f64[0], xmm0.m256d_f64[1], xmm0.m256d_f64[2]);
}

float64 vec3d::Dot(const vec3d& v) const {
	__m256d xmm = _mm256_set_pd(0, v.z, v.y, v.x);
	xmm = _mm256_mul_pd(xmm, _mm256_set_pd(0, y, z, x));
	return xmm.m256d_f64[0] + xmm.m256d_f64[1] + xmm.m256d_f64[2];
}

}
}
}