#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>
#include <core/math/mat4.h>
#include <core/video/window.h>
#include <core/log/log.h>
#include <core/log/logdevice_console.h>
#include <stdio.h>
using namespace fd::core::math;
using namespace fd::core::video;
using namespace fd::utils;
using namespace fd::core;
using namespace fd::core::log;

int main() {

	vec3d test(1, 0, 0);

	test = test.Cross(vec3d(0, 1, 0));

	printf("%f %f %f\n", test.x, test.y, test.z);

	Log::AddDevice(new LogDeviceConsole());

	Factory::CreateFactory();

	WindowCreateInfo info;

	info.graphicsAdapter = nullptr;
	info.outputWindow = nullptr;
	info.width = 1280;
	info.height = 720;
	info.refreshRate = 60;
	info.title = "Dank Title";

	Window window(&info);

	while (window.IsOpen()) {

		window.Update();
	}

}