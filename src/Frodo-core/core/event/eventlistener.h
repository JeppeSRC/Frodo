#pragma once

#include "event.h"

namespace fd {
namespace core {
namespace event {

enum EventListenerTypes {
	EventWindow = 0x01,
	EventMouse = 0x02,
	EventKeyboard = 0x04,

	EventAll = EventWindow | EventMouse | EventKeyboard,
	EventInput = EventMouse | EventKeyboard
};

class EventListener {
protected:
	EventListenerTypes events;

	bool registered;

	EventListener(EventListenerTypes events, bool autoRegister = true);
	~EventListener();

	void Register();
	void UnRegister();
public:
	virtual bool OnEvent(const Event* const event) { return false; }
			
	virtual bool OnWindowEvent(const WindowEvent* const event) { return false; }
	virtual bool OnWindowEventResize(const math::vec2i& size) { return false; }
	virtual bool OnWindowEventMove(const math::vec2i& position) { return false; }
	virtual bool OnWindowEventFocus(bool focus) { return false; }
	virtual bool OnWindowEventVisiblity(bool visibility) { return false; }
			
	virtual bool OnKeyboardEvent(const KeyboardEvent* const event) { return false; }
	virtual bool OnKeyboardEventPressed(uint32 key) { return false; }
	virtual bool OnKeyboardEventReleased(uint32 key) { return false; }
	virtual bool OnKeyboardEventHold(uint32 key) { return false; }
			
	virtual bool OnMouseEvent(const MouseEvent* const event) { return false; }
	virtual bool OnMouseEventMove(const math::vec2i& absolute, const math::vec2i& relative) { return false; }
	virtual bool OnMouseEventPressed(uint32 button) { return false; }
	virtual bool OnMouseEventReleased(uint32 button) { return false; }
	virtual bool OnMouseEventHold(uint32 button) { return false; }
};

}
}
}