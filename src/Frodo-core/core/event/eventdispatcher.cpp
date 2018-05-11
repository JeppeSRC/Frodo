#include "eventdispatcher.h"
#include <core/log/log.h>
#include <core/video/context.h>

namespace fd {
namespace core {
namespace event {

using namespace utils;
using namespace log;
using namespace video;

List<EventListener*> EventDispatcher::allListeners;
List<EventListener*> EventDispatcher::windowListeners;
List<EventListener*> EventDispatcher::mouseListeners;
List<EventListener*> EventDispatcher::keyboardListeners;

void EventDispatcher::DispatchEvent(const Event* const event) {
	OnEvent(event);
}

bool EventDispatcher::OnEvent(const Event* const event) {
	uint_t size = allListeners.GetSize();

	for (uint_t i = 0; i < size; i++) {
		EventListener* listener = allListeners[i];

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

	if (event->type == EventType::Window) {
		size = windowListeners.GetSize();

		WindowEvent* e = (WindowEvent*)event;

		for (uint_t i = 0; i < size; i++) {
			EventListener* listener = windowListeners[i];

			OnWindowEvent(listener, (WindowEvent*)event);
		}
	} else if (event->type == EventType::InputMouse) {
		size = mouseListeners.GetSize();

		for (uint_t i = 0; i < size; i++) {
			EventListener* listener = mouseListeners[i];

			OnMouseEvent(listener, (MouseEvent*)event);
		}
	} else if (event->type == EventType::InputKeyboard) {
		size = keyboardListeners.GetSize();

		for (uint_t i = 0; i < size; i++) {
			EventListener* listener = keyboardListeners[i];

			OnKeyboardEvent(listener, (KeyboardEvent*)event);
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

void EventDispatcher::RegisterListener(EventListener* listener, EventListenerTypes events) {
	
	if (events == EventAll) {
		allListeners.Push_back(listener);
		return;
	}

	if (events & EventWindow) {
		windowListeners.Push_back(listener);
	} 

	if (events & EventMouse) {
		mouseListeners.Push_back(listener);
	} 

	if (events & EventKeyboard) {
		keyboardListeners.Push_back(listener);
	}
}

void EventDispatcher::UnRegisterListener(EventListener* listener, EventListenerTypes events) {
	if (events == EventAll) {
		allListeners.Remove(listener);
		return;
	}

	if (events & EventWindow) {
		windowListeners.Remove(listener);
	}

	if (events & EventMouse) {
		mouseListeners.Remove(listener);
	}

	if (events & EventKeyboard) {
		keyboardListeners.Remove(listener);
	}
}

}
}
}