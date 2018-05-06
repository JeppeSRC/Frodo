#include "window.h"


namespace fd {
namespace core {
namespace video {

Window::Window(WindowCreateInfo* info) : info(*info), open(false) { }

Window* Window::Create(WindowCreateInfo* info) {
#ifdef FD_WINDOWS
	return new WINWindow(info);
#elif FD_LINUX
	return nullptr;
#endif
}

}
}
}
