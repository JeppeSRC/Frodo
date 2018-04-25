#include "application.h"
#include <core/log/log.h>
#include <core/log/logdevice_console.h>
#include <core/log/logdevice_file.h>
#include <ctime>

namespace fd {
namespace core {
namespace app {

using namespace utils;
using namespace video;
using namespace log;

Application::Application(const String& name) {
	Log::AddDevice(new LogDeviceConsole);
}

Application::~Application() {
	delete window;
}

void Application::OnInitInternal() {
	Factory::CreateFactory();

	OnInit();
}

void Application::Start() {
	WindowCreateInfo winfo;
	OnWindowCreate(&winfo);
	window = Window::Create(&winfo);

	OnInitInternal();

	uint32 lastTime = clock();
	uint32 lastTime2 = clock();

	while (running) {
		uint32 now = clock();
		float delta = float(now - lastTime) / 1000.0f;
		lastTime = now;

		OnUpdate(delta);
		OnRender();

		if ((now = clock()) - lastTime2 > 1000) {
			OnTick();
			lastTime2 = now;
		}

	}

	OnExit();
}

}
}
}