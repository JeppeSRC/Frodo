#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>
#include <stdio.h>
using namespace fd::core::math;

int main() {

	vec4 test(2, 60, 21, 128);
	vec4 test2(1, 30, 21, 64);

	test.Div(test2);

	printf("%f %f %f %f\n", test.x, test.y, test.z, test.w);
}