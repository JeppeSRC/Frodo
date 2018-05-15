#pragma once

#include <utils/string.h>
#include <core/video/window.h>

namespace fd {
namespace core {
namespace app {

class Application {
private:
	void OnInitInternal();
protected:
	virtual void OnWindowCreate(video::WindowCreateInfo* info) = 0;
	virtual void OnInit() { }
	virtual void OnTick() { }
	virtual void OnUpdate(float delta) { }
	virtual void OnRender() = 0;
	virtual void OnExit() {}
	

	utils::String appName;

	video::Window* window;

	bool running;
public:
	Application(const utils::String& name);
	~Application();

	void Start();

};

} } }