#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>
#include <core/math/mat4.h>
#include <stdio.h>
using namespace fd::core::math;

int main() {

	vec3 test(1, 2, 3);

	test = mat4::Translate(vec3(1, 0, 0)) * test;

	printf("%f %f %f\n", test.x, test.y, test.z);
}