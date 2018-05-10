#include "eventdispatcher.h"
#include <core/log/log.h>

namespace fd {
namespace core {
namespace event {

using namespace utils;
using namespace log;

List<EventListener*> EventDispatcher::listeners;

void EventDispatcher::DispatchEvent(const Event* const event) {
	OnEvent(event);
}

bool EventDispatcher::OnEvent(const Event* const event) {
	uint_t size = listeners.GetSize();

	for (uint_t i = 0; i < size; i++) {
		EventListener* listener = listeners[i];

		bool handled = listener->OnEvent(event);

		if (handled) continue;

		switch (event->type) {
			case EventType::Unknown:
				Log::Warning("[EventDispatcher] Received unknown event");
				return false;
			case EventType::InputKeyboard:
				handled = OnKeyboardEvent(listener, (KeyboardEvent*)event);
				break;
			case EventType::InputMouse:
				handled = OnMouseEvent(listener, (MouseEvent*)event);
				break;
			case EventType::Window:
				handled = OnWindowEvent(listener, (WindowEvent*)event);
				break;
		}
	}
}

bool EventDispatcher::OnWindowEvent(EventListener* const listener, const WindowEvent* const event) {
	if (listener->OnWindowEvent(event)) return true;

	bool handled = false;

	switch (event->action) {
		case EventAction::Move:
			handled = listener->OnWindowEventMove(event->position);
			break;
		case EventAction::Resize:
			handled = listener->OnWindowEventResize(event->size);
			break;
		case EventAction::Focus:
			handled = listener->OnWindowEventFocus(event->focus);
			break;
		case EventAction::MinimizeMaximize:
			handled = listener->OnWindowEventVisiblity(event->visibility);
			break;
	}
	
	return handled;
}


bool EventDispatcher::OnKeyboardEvent(EventListener* const listener, const KeyboardEvent* const event) {
	if (listener->OnKeyboardEvent(event)) return true;

	bool handled = false;

	switch (event->action) {
		case EventAction::Pressed:
			handled = listener->OnKeyboardEventPressed(event->key);
			break;
		case EventAction::Released:
			handled = listener->OnKeyboardEventReleased(event->key);
			break;
	}

	return handled;
}


bool EventDispatcher::OnMouseEvent(EventListener* const listener, const MouseEvent* const event) {
	if (listener->OnMouseEvent(event)) return true;

	bool handled = false;

	switch (event->action) {
		case EventAction::Move:
			handled = listener->OnMouseEventMove(event->absolute, event->relative);
			break;
		case EventAction::Pressed:
			handled = listener->OnMouseEventPressed(event->button);
			break;
		case EventAction::Released:
			handled = listener->OnMouseEventReleased(event->button);
			break;
	}

	return handled;
}

}
}
}