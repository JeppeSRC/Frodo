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
	virtual void OnWindowEventFocus(bool focus) { }
	virtual void OnWindowEventVisiblity(bool visibility) { }

	virtual void OnKeyboardEvent(const KeyboardEvent* const event) { }
	virtual void OnKeyboardEventPressed(uint32 key) { }
	virtual void OnKeyboardEventReleased(uint32 key) { }
	virtual void OnKeyboardEventHold(uint32 key) { }

	virtual void OnMouseEvent(const MouseEvent* const event) { }
	virtual void OnMouseEventMove(const math::vec2i& absolute, const math::vec2i& relative) { }
	virtual void OnMouseEventPressed(uint32 button) { }
	virtual void OnMouseEventReleased(uint32 button) { }
	virtual void OnMouseEventHold(uint32 button) { }
};

}
}
}