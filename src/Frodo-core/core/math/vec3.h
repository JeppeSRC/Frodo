#pragma once
#include "mathcommon.h"

namespace fd {
namespace core {
namespace math {

class vec3 {
public:
	float32 x;
	float32 y;
	float32 z;

public:
	vec3();
	vec3(float32 x, float32 y, float32 z);

	vec3& Add(const vec3& v);
	vec3& Add(float32 v);
	vec3& Sub(const vec3& v);
	vec3& Sub(float32 v);
	vec3& Mul(const vec3& v);
	vec3& Mul(float32 v);
	vec3& Div(const vec3& v);
	vec3& Div(float32 v);

	float32 Length() const;
	vec3& Normalize();
	vec3 Cross(const vec3& v) const;
	float32 Dot(const vec3& v) const;

	inline void operator+=(const vec3& right) { Add(right); }
	inline void operator-=(const vec3& right) { Sub(right); }
	inline void operator*=(const vec3& right) { Mul(right); }
	inline void operator/=(const vec3& right) { Div(right); }
	inline void operator+=(const float32 right) { Add(right); }
	inline void operator-=(const float32 right) { Sub(right); }
	inline void operator*=(const float32 right) { Mul(right); }
	inline void operator/=(const float32 right) { Div(right); }

	inline vec3 operator+(const vec3& right) const { return vec3(*this).Add(right); }
	inline vec3 operator-(const vec3& right) const { return vec3(*this).Sub(right); }
	inline vec3 operator*(const vec3& right) const { return vec3(*this).Mul(right); }
	inline vec3 operator/(const vec3& right) const { return vec3(*this).Div(right); }
	inline vec3 operator+(const float32 right) const { return vec3(*this).Add(right); }
	inline vec3 operator-(const float32 right) const { return vec3(*this).Sub(right); }
	inline vec3 operator*(const float32 right) const { return vec3(*this).Mul(right); }
	inline vec3 operator/(const float32 right) const { return vec3(*this).Div(right); }

	inline vec3 operator-() const { return vec3(-x, -y, -z); }
};

class vec3d {
public:
	float64 x;
	float64 y;
	float64 z;

public:
	vec3d();
	vec3d(float64 x, float64 y, float64 z);

	vec3d& Add(const vec3d& v);
	vec3d& Add(float64 v);
	vec3d& Sub(const vec3d& v);
	vec3d& Sub(float64 v);
	vec3d& Mul(const vec3d& v);
	vec3d& Mul(float64 v);
	vec3d& Div(const vec3d& v);
	vec3d& Div(float64 v);

	float64 Length() const;
	vec3d& Normalize();
	vec3d Cross(const vec3d& v) const;
	float64 Dot(const vec3d& v) const;

	inline void operator+=(const vec3d& right) { Add(right); }
	inline void operator-=(const vec3d& right) { Sub(right); }
	inline void operator*=(const vec3d& right) { Mul(right); }
	inline void operator/=(const vec3d& right) { Div(right); }
	inline void operator+=(const float64 right) { Add(right); }
	inline void operator-=(const float64 right) { Sub(right); }
	inline void operator*=(const float64 right) { Mul(right); }
	inline void operator/=(const float64 right) { Div(right); }

	inline vec3d operator+(const vec3d& right) const { return vec3d(*this).Add(right); }
	inline vec3d operator-(const vec3d& right) const { return vec3d(*this).Sub(right); }
	inline vec3d operator*(const vec3d& right) const { return vec3d(*this).Mul(right); }
	inline vec3d operator/(const vec3d& right) const { return vec3d(*this).Div(right); }
	inline vec3d operator+(const float64 right) const { return vec3d(*this).Add(right); }
	inline vec3d operator-(const float64 right) const { return vec3d(*this).Sub(right); }
	inline vec3d operator*(const float64 right) const { return vec3d(*this).Mul(right); }
	inline vec3d operator/(const float64 right) const { return vec3d(*this).Div(right); }

	inline vec3d operator-() const { return vec3d(-x, -y, -z); }
};

class vec3i {
public:
	int32 x;
	int32 y;
	int32 z;

public:
	vec3i();
	vec3i(int32 x, int32 y, int32 z);

	vec3i& Add(const vec3i& v);
	vec3i& Add(int32 v);
	vec3i& Sub(const vec3i& v);
	vec3i& Sub(int32 v);
	vec3i& Mul(const vec3i& v);
	vec3i& Mul(int32 v);
	vec3i& Div(const vec3i& v);
	vec3i& Div(int32 v);

	inline void operator+=(const vec3i& right) { Add(right); }
	inline void operator-=(const vec3i& right) { Sub(right); }
	inline void operator*=(const vec3i& right) { Mul(right); }
	inline void operator/=(const vec3i& right) { Div(right); }
	inline void operator+=(const int32 right) { Add(right); }
	inline void operator-=(const int32 right) { Sub(right); }
	inline void operator*=(const int32 right) { Mul(right); }
	inline void operator/=(const int32 right) { Div(right); }

	inline vec3i operator+(const vec3i& right) const { return vec3i(*this).Add(right); }
	inline vec3i operator-(const vec3i& right) const { return vec3i(*this).Sub(right); }
	inline vec3i operator*(const vec3i& right) const { return vec3i(*this).Mul(right); }
	inline vec3i operator/(const vec3i& right) const { return vec3i(*this).Div(right); }
	inline vec3i operator+(const int32 right) const { return vec3i(*this).Add(right); }
	inline vec3i operator-(const int32 right) const { return vec3i(*this).Sub(right); }
	inline vec3i operator*(const int32 right) const { return vec3i(*this).Mul(right); }
	inline vec3i operator/(const int32 right) const { return vec3i(*this).Div(right); }

	inline vec3i operator-() const { return vec3i(-x, -y, -z); }
};

class vec3l {
public:
	int64 x;
	int64 y;
	int64 z;

public:
	vec3l();
	vec3l(int64 x, int64 y, int64 z);

	vec3l& Add(const vec3l& v);
	vec3l& Add(int64 v);
	vec3l& Sub(const vec3l& v);
	vec3l& Sub(int64 v);
	vec3l& Mul(const vec3l& v);
	vec3l& Mul(int64 v);
	vec3l& Div(const vec3l& v);
	vec3l& Div(int64 v);

	inline void operator+=(const vec3l& right) { Add(right); }
	inline void operator-=(const vec3l& right) { Sub(right); }
	inline void operator*=(const vec3l& right) { Mul(right); }
	inline void operator/=(const vec3l& right) { Div(right); }
	inline void operator+=(const int64 right) { Add(right); }
	inline void operator-=(const int64 right) { Sub(right); }
	inline void operator*=(const int64 right) { Mul(right); }
	inline void operator/=(const int64 right) { Div(right); }

	inline vec3l operator+(const vec3l& right) const { return vec3l(*this).Add(right); }
	inline vec3l operator-(const vec3l& right) const { return vec3l(*this).Sub(right); }
	inline vec3l operator*(const vec3l& right) const { return vec3l(*this).Mul(right); }
	inline vec3l operator/(const vec3l& right) const { return vec3l(*this).Div(right); }
	inline vec3l operator+(const int64 right) const { return vec3l(*this).Add(right); }
	inline vec3l operator-(const int64 right) const { return vec3l(*this).Sub(right); }
	inline vec3l operator*(const int64 right) const { return vec3l(*this).Mul(right); }
	inline vec3l operator/(const int64 right) const { return vec3l(*this).Div(right); }

	inline vec3l operator-() const { return vec3l(-x, -y, -z); }
};

}
}
}