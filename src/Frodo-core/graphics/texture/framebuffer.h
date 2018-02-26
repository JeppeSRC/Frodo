#include "texture.h"
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace texture {

#if FD_DX 


#else

class Framebuffer : public Texture2D {
protected:

public:
	Framebuffer(uint32 width, uint32 height, Format format);
};

#endif

}
}
}