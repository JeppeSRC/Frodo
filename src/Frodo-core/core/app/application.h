#pragma once

#include <utils/string.h>
#include <core/video/window.h>

namespace fd {
namespace core {
namespace app {

class Application {
private:
	utils::String appName;

	void Run();
protected:
	virtual void OnWindowCreate(video::WindowCreateInfo* info) = 0;
	virtual void OnInit() = 0;
	virtual void OnTick() = 0;
	virtual void OnUpdate(float delta) = 0;
	virtual void OnRender() = 0;
	virtual void OnExit() = 0;

public:
	Application(const utils::String& name);
	~Application();

	void Start();

};

} } }