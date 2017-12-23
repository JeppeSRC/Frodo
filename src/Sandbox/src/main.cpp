#include <core/math/vec2.h>
#include <stdio.h>
using namespace fd::core::math;

int main() {

	vec2 test(2, 0.1);
	vec2 test2(1, 30);

	test.Div(test2);

	printf("%f %f\n", test.x, test.y);
}