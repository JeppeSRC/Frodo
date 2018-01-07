#pragma once

#include <core/enums.h>
#include <utils/string.h>
#include <utils/list.h>

#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>

namespace fd {
namespace graphics {
namespace buffer {

struct BufferLayoutAttrib {
	utils::String name;
	BufferLayoutAttribType type;
};

class BufferLayout {
private:
	utils::List<BufferLayoutAttrib> attribs;

	uint32 inputSlot;
	uint32 stride;
	BufferInputRate rate;
public:
	BufferLayout(uint32 inputSlot, BufferInputRate rate);
	
	static uint32 GetSize(BufferLayoutAttribType type);

	template<typename T>
	void Push(const utils::String& name);

	inline const utils::List<BufferLayoutAttrib>& GetAttribs() const { return attribs; }
	inline uint32 GetInputSlot() const { return inputSlot; }
	inline uint32 GetStride() const { return stride; }
	inline BufferInputRate GetInputRate() const { return rate; }

};

} } }