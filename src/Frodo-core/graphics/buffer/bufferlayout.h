#pragma once

#include <core/enums.h>
#include <utils/string.h>
#include <utils/list.h>

namespace fd {
namespace graphics {
namespace buffer {

struct BufferLayoutAttrib {
	utils::String name;
	BufferLayoutAttribType type;
	uint32 inputSlot;
};

class BufferLayout {
private:

	static uint32 GetSize(BufferLayoutAttribType type);

private:
	utils::List<BufferLayoutAttrib> attribs;

	uint32 stride;
public:
	BufferLayout();
	

	template<typename T>
	void Push(const utils::String& name, uint32 inputSlot) {   }

	inline const utils::List<BufferLayoutAttrib>& GetAttribs() const { return attribs; }
	inline uint32 GetStride() const { return stride; }
};

} } }