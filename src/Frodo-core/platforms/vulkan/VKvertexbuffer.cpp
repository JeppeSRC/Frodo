#include <graphics/buffer/vertexbuffer.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace buffer {

using namespace core;
using namespace video;

VertexBuffer::VertexBuffer(byte* const data, uint64 size) {
	VkBufferCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	info.size = size;
	info.queueFamilyIndexCount = 0;
	info.pQueueFamilyIndices = nullptr;

	VK(vkCreateBuffer(Context::GetDevice(), &info, nullptr, &buf));

	VkMemoryRequirements req;

	vkGetBufferMemoryRequirements(Context::GetDevice(), buf, &req);

	const VkPhysicalDeviceMemoryProperties mem = Context::GetAdapter()->GetMemoryProperties();

	uint32 i;

	for (i = 0; i < mem.memoryTypeCount; i++) {
		if (req.memoryTypeBits & (1 << i) && (mem.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) == (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
			break;
	}

	VkMemoryAllocateInfo ainfo;

	ainfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	ainfo.pNext = nullptr;
	ainfo.memoryTypeIndex = i;
	ainfo.allocationSize = req.size;

	VK(vkAllocateMemory(Context::GetDevice(), &ainfo, nullptr, &deviceMemory));

	VK(vkBindBufferMemory(Context::GetDevice(), buf, deviceMemory, 0));
}

VertexBuffer::~VertexBuffer() {
	vkDestroyBuffer(Context::GetDevice(), buf, nullptr);
	vkFreeMemory(Context::GetDevice(), deviceMemory, nullptr);
}

} } }