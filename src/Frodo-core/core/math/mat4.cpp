#include "mat4.h"
#include <memory>

namespace fd {
namespace core {
namespace math {

void mat4::LoadRows(__m128* rows) const {
	rows[0] = _mm_set_ps(m[0 + 3 * 4], m[0 + 2 * 4], m[0 + 1 * 4], m[0 + 0 * 4]);
	rows[1] = _mm_set_ps(m[1 + 3 * 4], m[1 + 2 * 4], m[1 + 1 * 4], m[1 + 0 * 4]);
	rows[2] = _mm_set_ps(m[2 + 3 * 4], m[2 + 2 * 4], m[2 + 1 * 4], m[2 + 0 * 4]);
	rows[3] = _mm_set_ps(m[3 + 3 * 4], m[3 + 2 * 4], m[3 + 1 * 4], m[3 + 0 * 4]);
}

void mat4::LoadColumns(__m128* columns) const {
	columns[0] = _mm_loadu_ps(m);
	columns[1] = _mm_loadu_ps(m+4);
	columns[2] = _mm_loadu_ps(m+8);
	columns[3] = _mm_loadu_ps(m+12);
}

mat4::mat4() {
	memset(m, 0, sizeof(m));
}

mat4::mat4(float32 diagonal) : mat4() {
	m[0 + 0 * 4] = diagonal;
	m[1 + 1 * 4] = diagonal;
	m[2 + 2 * 4] = diagonal;
	m[3 + 3 * 4] = diagonal;
}

mat4 mat4::Scale(const vec3& v) {
	mat4 m;

	m.m[0 + 0 * 4] = v.x;
	m.m[1 + 1 * 4] = v.y;
	m.m[2 + 2 * 4] = v.z;
	m.m[3 + 3 * 4] = 1;

	return m;
}

mat4 mat4::Translate(const vec3& v) {
	mat4 m(1);

	m.m[3 + 0 * 4] = v.x;
	m.m[3 + 1 * 4] = v.y;
	m.m[3 + 2 * 4] = v.z;

	return m;
}

mat4 mat4::Rotate(const vec3& v) {
	mat4 x(1), y(1), z(1);

	float32 xcos = cosf((float32)FD_TO_RADIANS_F(v.x));
	float32 xsin = sinf((float32)FD_TO_RADIANS_F(v.x));
	float32 ycos = cosf((float32)FD_TO_RADIANS_F(v.y));
	float32 ysin = sinf((float32)FD_TO_RADIANS_F(v.y));
	float32 zcos = cosf((float32)FD_TO_RADIANS_F(v.z));
	float32 zsin = sinf((float32)FD_TO_RADIANS_F(v.z));

	x.m[1 + 1 * 4] = xcos; x.m[1 + 2 * 4] = -xsin;
	x.m[2 + 1 * 4] = xsin; x.m[2 + 2 * 4] = xcos;

	y.m[0 + 0 * 4] = ycos; y.m[0 + 2 * 4] = -ysin;
	y.m[2 + 0 * 4] = ysin; y.m[2 + 2 * 4] = ycos;

	z.m[0 + 0 * 4] = zcos; z.m[0 + 1 * 4] = -zsin;
	z.m[1 + 0 * 4] = zsin; z.m[1 + 1 * 4] = zcos;

	return x * y * z;
}

mat4 mat4::Perspective(float32 aspect, float32 fov, float32 zNear, float32 zFar) {
	mat4 m(1);

	

	m.m[0 + 0 * 4] = 1.0f / (aspect * (tanh(fov * 0.5f)));
	m.m[1 + 1 * 4] = 1.0f / (tanh(fov * 0.5f));
	m.m[2 + 2 * 4] = zFar / (zFar - zNear);
	m.m[3 + 2 * 4] = 1;
	m.m[2 + 3 * 4] = -zNear * (zFar / (zFar - zNear));

	return m;
}

mat4 mat4::operator*(const mat4& r) const {
	mat4 tmp;

	__m128 row[4];
	__m128 col[4];

	r.LoadColumns(col);
	LoadRows(row);

	__m128 res;

	for (uint_t y = 0; y < 4; y++) {
		for (uint_t x = 0; x < 4; x++) {
			res = _mm_mul_ps(row[x], col[y]);
			tmp.m[x + y * 4] = res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2] + res.m128_f32[3];
		}
	}

	return tmp;
}

vec3 mat4::operator*(const vec3& v) const {
	__m128 row[4];
	__m128 col;

	LoadRows(row);

	col = _mm_set_ps(0, v.z, v.y, v.x);


	__m128 res = _mm_mul_ps(row[0], col);

	for (uint_t i = 1; i < 4; i++)
		res = _mm_fmadd_ps(row[i], col, res);

	return vec3(res.m128_f32[0], res.m128_f32[1], res.m128_f32[2]);
}

vec4 mat4::operator*(const vec4& v) const {
	__m128 row[4];
	__m128 col;

	LoadRows(row);

	col = _mm_set_ps(v.w, v.z, v.y, v.x);


	__m128 res = _mm_mul_ps(row[0], col);

	for (uint_t i = 1; i < 4; i++)
		res = _mm_fmadd_ps(row[i], col, res);

	return vec4(res.m128_f32[0], res.m128_f32[1], res.m128_f32[2], res.m128_f32[3]);
}

} } }