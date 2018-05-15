#pragma once
#include "event.h"
#include "eventlistener.h"

#include <utils/list.h>

namespace fd {
namespace core {
namespace event {

class EventDispatcher {
private:
	static utils::List<EventListener*> allListeners;
	static utils::List<EventListener*> windowListeners;
	static utils::List<EventListener*> mouseListeners;
	static utils::List<EventListener*> keyboardListeners;

	static bool OnEvent(const Event* const event);
	static bool OnWindowEvent(EventListener* const listener, const WindowEvent* const event);
	static bool OnKeyboardEvent(EventListener* const listener, const KeyboardEvent* const event);
	static bool OnMouseEvent(EventListener* const listener, const MouseEvent* const event);

public:
	static void DispatchEvent(const Event* const event);
	static void RegisterListener(EventListener* listener, EventListenerTypes events);
	static void UnRegisterListener(EventListener* listener, EventListenerTypes events);
};

}
}
}
