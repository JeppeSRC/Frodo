#pragma once

#include <core/types.h>
#include <utils/string.h>



namespace fd {
namespace core {
namespace video {

class Window {
private:
	uint32 width;
	uint32 height;

	utils::String title;

#ifdef FD_WINDOWS
	HWND hwmd;
#endif

public:
	
	inline uint32 GetWidth() const { return width; }
	inline uint32 GetHeight() const { return height; }
	inline utils::String GetTitle() const { return title; }
};

} } }