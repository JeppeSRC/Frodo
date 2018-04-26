#include "texture.h"
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace texture {

class Framebuffer : public Texture2D {
protected:


public:
	Framebuffer(uint32 width, uint32 height, Format format);

};

}
}
}