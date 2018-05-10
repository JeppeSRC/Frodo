#pragma once

#include "event.h"

namespace fd {
namespace core {
namespace event {

class EventListener {
public:
	virtual bool OnEvent(const Event* const event) { }
			
	virtual bool OnWindowEvent(const WindowEvent* const event) { }
	virtual bool OnWindowEventResize(const math::vec2i& size) { }
	virtual bool OnWindowEventMove(const math::vec2i& position) { }
	virtual bool OnWindowEventFocus(bool focus) { }
	virtual bool OnWindowEventVisiblity(bool visibility) { }
			
	virtual bool OnKeyboardEvent(const KeyboardEvent* const event) { }
	virtual bool OnKeyboardEventPressed(uint32 key) { }
	virtual bool OnKeyboardEventReleased(uint32 key) { }
	virtual bool OnKeyboardEventHold(uint32 key) { }
			
	virtual bool OnMouseEvent(const MouseEvent* const event) { }
	virtual bool OnMouseEventMove(const math::vec2i& absolute, const math::vec2i& relative) { }
	virtual bool OnMouseEventPressed(uint32 button) { }
	virtual bool OnMouseEventReleased(uint32 button) { }
	virtual bool OnMouseEventHold(uint32 button) { }
};

}
}
}