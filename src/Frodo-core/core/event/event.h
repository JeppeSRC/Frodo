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

	Event* event;

	Event(EventType type) : type(type), event(this) {}
};


// Window events
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

	MouseEvent(EventAction action, math::vec2i absolute, math::vec2i relative, uint32 button) : Event(EventType::InputMouse), action(action), absolute(absolute), relative(relative), button(button) { }
};

}
}
}

