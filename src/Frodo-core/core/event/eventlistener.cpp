#include "eventlistener.h"
#include "eventdispatcher.h"
#include <core/log/log.h>

namespace fd {
namespace core {
namespace event {

using namespace log;

EventListener::EventListener(EventListenerTypes events, bool autoRegister) : events(events), registered(autoRegister) {
	if (autoRegister) EventDispatcher::RegisterListener(this, events);
}

EventListener::~EventListener() {
	UnRegister();
}

void EventListener::Register() {
	if (!registered) EventDispatcher::RegisterListener(this, events);
}

void EventListener::UnRegister() {
	if (registered) EventDispatcher::UnRegisterListener(this, events);
}

}
}
}