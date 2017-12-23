#pragma once

#include "mathcommon.h"
#include "vec3.h"
#include "vec4.h"

namespace fd {
namespace core {
namespace math {

class mat4 {
private:
	void LoadRows(__m128* rows) const;
	void LoadColumns(__m128* columns) const;
public:
	float32 m[16];

public:
	mat4();
	mat4(float32 digonal);

	static mat4 Scale(const vec3& v);
	static mat4 Translate(const vec3& v);
	static mat4 Rotate(const vec3& v);

	static mat4 Perspective(float32 aspect, float32 fov, float32 zNear, float32 zFar);

	mat4 operator*(const mat4& r) const;

	vec3 operator*(const vec3& v) const;
	vec4 operator*(const vec4& v) const;
};

} } }