#include "bufferlayout.h"
#include <core/log/log.h>

#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>

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

BufferLayout::BufferLayout() {

}

template<> void BufferLayout::Push<float32>(const String& name, uint32 inputSlot) {
	attribs.Push_back({ name, BufferLayoutAttribType::Float, inputSlot });
	stride += 4;
}

template<> void BufferLayout::Push<core::math::vec2>(const String& name, uint32 inputSlot) {
	attribs.Push_back({ name, BufferLayoutAttribType::Vec2, inputSlot });
	stride += 8;
}

template<> void BufferLayout::Push<core::math::vec3>(const String& name, uint32 inputSlot) {
	attribs.Push_back({ name, BufferLayoutAttribType::Vec3, inputSlot });
	stride += 12;
}

template<> void BufferLayout::Push<core::math::vec4>(const String& name, uint32 inputSlot) {
	attribs.Push_back({ name, BufferLayoutAttribType::Vec4, inputSlot });
	stride += 16;
}

}}}