#pragma once
#include "mathcommon.h"

namespace fd {
namespace core {
namespace math {

class vec4 {
public:
	float32 x;
	float32 y;
	float32 z;
	float32 w;

public:
	vec4();
	vec4(float32 x, float32 y, float32 z, float32 w);

	vec4& Add(const vec4& v);
	vec4& Add(float32 v);
	vec4& Sub(const vec4& v);
	vec4& Sub(float32 v);
	vec4& Mul(const vec4& v);
	vec4& Mul(float32 v);
	vec4& Div(const vec4& v);
	vec4& Div(float32 v);
};

class vec4d {
public:
	float64 x;
	float64 y;
	float64 z;
	float64 w;

public:
	vec4d();
	vec4d(float64 x, float64 y, float64 z, float64 w);

	vec4d& Add(const vec4d& v);
	vec4d& Add(float64 v);
	vec4d& Sub(const vec4d& v);
	vec4d& Sub(float64 v);
	vec4d& Mul(const vec4d& v);
	vec4d& Mul(float64 v);
	vec4d& Div(const vec4d& v);
	vec4d& Div(float64 v);
};

class vec4i {
public:
	int32 x;
	int32 y;
	int32 z;
	int32 w;

public:
	vec4i();
	vec4i(int32 x, int32 y, int32 z, int32 w);

	vec4i& Add(const vec4i& v);
	vec4i& Add(int32 v);
	vec4i& Sub(const vec4i& v);
	vec4i& Sub(int32 v);
	vec4i& Mul(const vec4i& v);
	vec4i& Mul(int32 v);
	vec4i& Div(const vec4i& v);
	vec4i& Div(int32 v);
};

class vec4l {
public:
	int64 x;
	int64 y;
	int64 z;
	int64 w;

public:
	vec4l();
	vec4l(int64 x, int64 y, int64 z, int64 w);

	vec4l& Add(const vec4l& v);
	vec4l& Add(int64 v);
	vec4l& Sub(const vec4l& v);
	vec4l& Sub(int64 v);
	vec4l& Mul(const vec4l& v);
	vec4l& Mul(int64 v);
	vec4l& Div(const vec4l& v);
	vec4l& Div(int64 v);
};

}
}
}