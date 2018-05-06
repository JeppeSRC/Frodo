#pragma once

#include <core/math/vec2.h>

namespace fd {
namespace core {
namespace event {

enum class EventType {
	Unknown,
	InputMouse,
	InputKeyboard,
	Window,
};

enum class EventAction {
	Unknown,
	Pressed,
	Released,
	Hold,
	Resize,
	Move,
	MinimizeMaximize,
	Focus
};

class Event {
public:
	EventType type;

	Event(EventType type) : type(type) {}
};

class WindowEvent : public Event {
public:
	EventAction action;

	WindowEvent(EventAction action) : Event(EventType::Window), action(action) { }
};

class WindowEventResize : public WindowEvent {
public:
	math::vec2i size;

	WindowEventResize(const math::vec2i& size) : WindowEvent(EventAction::Resize), size(size) { }
};

class WindowEventMove : public WindowEvent {
public:
	math::vec2i position;

	WindowEventMove(const math::vec2i& position) : WindowEvent(EventAction::Move), position(position) { }
};

class WindowEventFocus : public WindowEvent {
public:
	bool focus;

	WindowEventFocus(bool value) : WindowEvent(EventAction::Focus), focus(focus) { }
};

class WindowEventVisibility : public WindowEvent {
public:
	bool visible;

	WindowEventVisibility(bool visible) : WindowEvent(EventAction::MinimizeMaximize), visible(visible) { }
};

}
}
}

