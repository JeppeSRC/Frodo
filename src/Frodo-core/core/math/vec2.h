#pragma once
#include "mathcommon.h"

namespace fd {
namespace core {
namespace math {

class vec2 {
public:
	float32 x;
	float32 y;

public:
	vec2();
	vec2(float32 x, float32 y);

	vec2& Add(const vec2& v);
	vec2& Add(float32 v);
	vec2& Sub(const vec2& v);
	vec2& Sub(float32 v);
	vec2& Mul(const vec2& v);
	vec2& Mul(float32 v);
	vec2& Div(const vec2& v);
	vec2& Div(float32 v);

	inline void operator+=(const vec2& right) { Add(right); }
	inline void operator-=(const vec2& right) { Sub(right); }
	inline void operator*=(const vec2& right) { Mul(right); }
	inline void operator/=(const vec2& right) { Div(right); }
	inline void operator+=(const float32 right) { Add(right); }
	inline void operator-=(const float32 right) { Sub(right); }
	inline void operator*=(const float32 right) { Mul(right); }
	inline void operator/=(const float32 right) { Div(right); }

	inline vec2 operator+(const vec2& right) const { return vec2(*this).Add(right); }
	inline vec2 operator-(const vec2& right) const { return vec2(*this).Sub(right); }
	inline vec2 operator*(const vec2& right) const { return vec2(*this).Mul(right); }
	inline vec2 operator/(const vec2& right) const { return vec2(*this).Div(right); }
	inline vec2 operator+(const float32 right) const { return vec2(*this).Add(right); }
	inline vec2 operator-(const float32 right) const { return vec2(*this).Sub(right); }
	inline vec2 operator*(const float32 right) const { return vec2(*this).Mul(right); }
	inline vec2 operator/(const float32 right) const { return vec2(*this).Div(right); }

	inline vec2 operator-() const { return vec2(-x, -y); }
};

class vec2d {
public:
	float64 x;
	float64 y;

public:
	vec2d();
	vec2d(float64 x, float64 y);

	vec2d& Add(const vec2d& v);
	vec2d& Add(float64 v);
	vec2d& Sub(const vec2d& v);
	vec2d& Sub(float64 v);
	vec2d& Mul(const vec2d& v);
	vec2d& Mul(float64 v);
	vec2d& Div(const vec2d& v);
	vec2d& Div(float64 v);

	inline void operator+=(const vec2d& right) { Add(right); }
	inline void operator-=(const vec2d& right) { Sub(right); }
	inline void operator*=(const vec2d& right) { Mul(right); }
	inline void operator/=(const vec2d& right) { Div(right); }
	inline void operator+=(const float64 right) { Add(right); }
	inline void operator-=(const float64 right) { Sub(right); }
	inline void operator*=(const float64 right) { Mul(right); }
	inline void operator/=(const float64 right) { Div(right); }

	inline vec2d operator+(const vec2d& right) const { return vec2d(*this).Add(right); }
	inline vec2d operator-(const vec2d& right) const { return vec2d(*this).Sub(right); }
	inline vec2d operator*(const vec2d& right) const { return vec2d(*this).Mul(right); }
	inline vec2d operator/(const vec2d& right) const { return vec2d(*this).Div(right); }
	inline vec2d operator+(const float64 right) const { return vec2d(*this).Add(right); }
	inline vec2d operator-(const float64 right) const { return vec2d(*this).Sub(right); }
	inline vec2d operator*(const float64 right) const { return vec2d(*this).Mul(right); }
	inline vec2d operator/(const float64 right) const { return vec2d(*this).Div(right); }

	inline vec2d operator-() const { return vec2d(-x, -y); }
};

class vec2i {
public:
	int32 x;
	int32 y;

public:
	vec2i();
	vec2i(int32 x, int32 y);

	vec2i& Add(const vec2i& v);
	vec2i& Add(int32 v);
	vec2i& Sub(const vec2i& v);
	vec2i& Sub(int32 v);
	vec2i& Mul(const vec2i& v);
	vec2i& Mul(int32 v);
	vec2i& Div(const vec2i& v);
	vec2i& Div(int32 v);

	inline void operator+=(const vec2i& right) { Add(right); }
	inline void operator-=(const vec2i& right) { Sub(right); }
	inline void operator*=(const vec2i& right) { Mul(right); }
	inline void operator/=(const vec2i& right) { Div(right); }
	inline void operator+=(const int32 right) { Add(right); }
	inline void operator-=(const int32 right) { Sub(right); }
	inline void operator*=(const int32 right) { Mul(right); }
	inline void operator/=(const int32 right) { Div(right); }

	inline vec2i operator+(const vec2i& right) const { return vec2i(*this).Add(right); }
	inline vec2i operator-(const vec2i& right) const { return vec2i(*this).Sub(right); }
	inline vec2i operator*(const vec2i& right) const { return vec2i(*this).Mul(right); }
	inline vec2i operator/(const vec2i& right) const { return vec2i(*this).Div(right); }
	inline vec2i operator+(const int32 right) const { return vec2i(*this).Add(right); }
	inline vec2i operator-(const int32 right) const { return vec2i(*this).Sub(right); }
	inline vec2i operator*(const int32 right) const { return vec2i(*this).Mul(right); }
	inline vec2i operator/(const int32 right) const { return vec2i(*this).Div(right); }

	inline vec2i operator-() const { return vec2i(-x, -y); }
};

class vec2l {
public:
	int64 x;
	int64 y;

public:
	vec2l();
	vec2l(int64 x, int64 y);

	vec2l& Add(const vec2l& v);
	vec2l& Add(int64 v);
	vec2l& Sub(const vec2l& v);
	vec2l& Sub(int64 v);
	vec2l& Mul(const vec2l& v);
	vec2l& Mul(int64 v);
	vec2l& Div(const vec2l& v);
	vec2l& Div(int64 v);

	inline void operator+=(const vec2l& right) { Add(right); }
	inline void operator-=(const vec2l& right) { Sub(right); }
	inline void operator*=(const vec2l& right) { Mul(right); }
	inline void operator/=(const vec2l& right) { Div(right); }
	inline void operator+=(const int64 right) { Add(right); }
	inline void operator-=(const int64 right) { Sub(right); }
	inline void operator*=(const int64 right) { Mul(right); }
	inline void operator/=(const int64 right) { Div(right); }

	inline vec2l operator+(const vec2l& right) const { return vec2l(*this).Add(right); }
	inline vec2l operator-(const vec2l& right) const { return vec2l(*this).Sub(right); }
	inline vec2l operator*(const vec2l& right) const { return vec2l(*this).Mul(right); }
	inline vec2l operator/(const vec2l& right) const { return vec2l(*this).Div(right); }
	inline vec2l operator+(const int64 right) const { return vec2l(*this).Add(right); }
	inline vec2l operator-(const int64 right) const { return vec2l(*this).Sub(right); }
	inline vec2l operator*(const int64 right) const { return vec2l(*this).Mul(right); }
	inline vec2l operator/(const int64 right) const { return vec2l(*this).Div(right); }

	inline vec2l operator-() const { return vec2l(-x, -y); }
};



}}}
