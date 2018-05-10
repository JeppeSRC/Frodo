#include "eventlistener.h"
#include "eventdispatcher.h"

namespace fd {
namespace core {
namespace event {

EventListener::EventListener(EventListenerTypes events) : events(events) {
	EventDispatcher::RegisterListener(this, events);
}

EventListener::~EventListener() {
	EventDispatcher::UnRegisterListener(this, events);
}

}
}
}