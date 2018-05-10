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

	Event(EventType type) : type(type), event(this) {}
};

// Window events
class WindowEvent : public Event {
public:
	EventAction action;

	math::vec2i size;
	math::vec2i position;

	bool focus;
	bool visibility;

	WindowEvent(EventAction action, const math::vec2i& size, const math::vec2i& position, bool focus, bool visibility) : Event(EventType::Window), action(action), size(size), position(position), focus(focus), visibility(visibility) {}
};

// Keyboard events
class KeyboardEvent : public Event {
public:
	EventAction action;

	uint32 key;

	KeyboardEvent(EventAction action, uint32 key) : Event(EventType::InputKeyboard), action(action), key(key) { }
};

//Nouse events
class MouseEvent : public Event {
public:
	EventAction action;

	math::vec2i absolute;
	math::vec2i relative;

	uint32 button;

	MouseEvent(EventAction action, const math::vec2i& absolute, const math::vec2i& relative, uint32 button) : Event(EventType::InputMouse), action(action), absolute(absolute), relative(relative), button(button) { }
};

}
}
}

