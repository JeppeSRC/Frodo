#include "texture.h"
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace texture {

class Framebuffer : public Texture2D {
protected:

	Framebuffer(uint32 width, uint32 height, Format format, VkImageUsageFlags usage, VkImageLayout layout);
public:
	Framebuffer(uint32 width, uint32 height, Format format, bool usedAsInput = false);

};

class Depthbuffer : public Framebuffer {
protected:

public:
	Depthbuffer(uint32 width, uint32 height, Format format, bool usedAsInput = false);
};

}
}
}