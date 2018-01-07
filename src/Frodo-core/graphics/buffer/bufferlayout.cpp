#include "bufferlayout.h"
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace buffer {

using namespace core::log;
using namespace utils;

uint32 BufferLayout::GetSize(BufferLayoutAttribType type) {
	switch (type) {
		case BufferLayoutAttribType::Float: return 4;
		case BufferLayoutAttribType::Vec2:  return 8;
		case BufferLayoutAttribType::Vec3:  return 12;
		case BufferLayoutAttribType::Vec4:  return 16;
	}

	FD_ASSERT(false);

	return 0;
}

BufferLayout::BufferLayout(uint32 inputSlot, BufferInputRate rate) : inputSlot(inputSlot), stride(0), rate(rate) {

}

template<> void BufferLayout::Push<float32>(const String& name) {
	attribs.Push_back({ name, BufferLayoutAttribType::Float });
	stride += 4;
}

template<> void BufferLayout::Push<core::math::vec2>(const String& name) {
	attribs.Push_back({ name, BufferLayoutAttribType::Vec2 });
	stride += 8;
}

template<> void BufferLayout::Push<core::math::vec3>(const String& name) {
	attribs.Push_back({ name, BufferLayoutAttribType::Vec3 });
	stride += 12;
}

template<> void BufferLayout::Push<core::math::vec4>(const String& name) {
	attribs.Push_back({ name, BufferLayoutAttribType::Vec4 });
	stride += 16;
}

}}}