#include "application.h"
#include <core/log/log.h>
#include <core/log/logdevice_console.h>
#include <core/log/logdevice_file.h>
#include <core/video/context.h>
#include <ctime>

namespace fd {
namespace core {
namespace app {

using namespace utils;
using namespace video;
using namespace log;

Application::Application(const String& name) {

}

Application::~Application() {
	delete window;
}

void Application::OnInitInternal() {
	Factory::CreateFactory();
	Log::AddDevice(new LogDeviceConsole);
}

void Application::Start() {
	OnInitInternal();
	WindowCreateInfo winfo;
	OnWindowCreate(&winfo);
	window = Window::Create(&winfo);
	OnInit();
	
	uint32 lastTime = clock();
	uint32 lastTime2 = clock();

	while (window->IsOpen()) {
		uint32 now = clock();
		float delta = float(now - lastTime) / 1000.0f;
		lastTime = now;

		OnUpdate(delta);
		OnRender();

		Context::Present(Context::GetCommandBuffers());

		if ((now = clock()) - lastTime2 > 1000) {
			OnTick();
			lastTime2 = now;
		}

		window->Update();
	}

	OnExit();
}

}
}
}