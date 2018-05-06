#pragma once

#include "event.h"

namespace fd {
namespace core {
namespace event {

class EventListener {
public:
	virtual void OnEvent(const Event* const event) { }

	virtual void OnWindowEvent(const WindowEvent* const event) { }
	virtual void OnWindowEventResize(const math::vec2i& size) { }
	virtual void OnWindowEventMove(const math::vec2i& position) { }
};

}
}
}