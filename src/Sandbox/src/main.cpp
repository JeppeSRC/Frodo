#include <core/video/context.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>
#include <core/math/mat4.h>
#include <core/video/window.h>
#include <core/log/log.h>
#include <core/log/logdevice_console.h>
#include <stdio.h>
#include <graphics/pipeline/pipeline.h>
#include <graphics/shader/shader.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <ctime>

using namespace fd;
using namespace core;
using namespace math;
using namespace log;
using namespace video;
using namespace utils;
using namespace graphics;
using namespace pipeline;
using namespace shader;
using namespace buffer;

struct Vertex {
	vec3 position;
	vec4 color;
};

int main() {

	vec3d test(1, 0, 0);

	test = test.Cross(vec3d(0, 1, 0));

	printf("%f %f %f\n", test.x, test.y, test.z);

	Log::AddDevice(new LogDeviceConsole());

	Factory::CreateFactory();

	WindowCreateInfo winfo;

	winfo.graphicsAdapter = Factory::GetAdapters()[0];
	winfo.outputWindow = nullptr;
	winfo.width = 1280;
	winfo.height = 720;
	winfo.refreshRate = 60;
	winfo.title = "Dank Title";

	Window window(&winfo);

	ViewportInfo viewInfo = { 0, 0, winfo.width, winfo.height, 0.0f, 1.0f };
	ScissorInfo scissorInfo = { 0, 0, viewInfo.width, viewInfo.height };
	BlendInfo blendInfo = { false, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add, ColorComponentFlag::All };
	DepthStencilInfo depthInfo = { };
	Shader shader("./res/vert.spv", "./res/frag.spv", "");

	BufferLayout inputLayout(0, BufferInputRate::PerVertex);

	inputLayout.Push<vec3>("POSITION");
	inputLayout.Push<vec4>("COLOR");

	PipelineInfo info;

	info.cullMode = CullMode::Back;
	info.frotFace = FrontFace::Clockwise;
	info.topology = PrimitiveTopology::TriangleList;
	info.polygonMode = PolygonMode::Fill;
	info.numScissors = 1;
	info.scissors = &scissorInfo;
	info.numViewports = 1;
	info.viewports = &viewInfo;
	info.numBlends = 1;
	info.blends = &blendInfo;
	info.shader = &shader;
	info.numInputLayouts = 1;
	info.shaderInputLayouts = &inputLayout;
	info.pipelineLayout.numElements = 0;
	info.depthStencilInfo = depthInfo;

	Pipeline pipeline(&info);

	Vertex vertices[3];

	vertices[0].position = vec3(0, -1, 0);
	vertices[0].color = vec4(1, 0, 1, 1);

	vertices[1].position = vec3(1, 1, 0);
	vertices[1].color = vec4(0, 1, 1, 1);

	vertices[2].position = vec3(-1, 1, 0);
	vertices[2].color = vec4(1, 1, 1, 1);

	uint32 indices[]{ 0, 1, 2 };

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, 3);

	const List<VkCommandBuffer>& cmd = Context::GetCmdBuffers();

	for (uint_t i = 0; i < cmd.GetSize(); i++) {
		VkCommandBufferBeginInfo info;

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pNext = nullptr;
		info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		info.pInheritanceInfo = nullptr;

		VK(vkBeginCommandBuffer(cmd[i], &info));

		VkClearValue gay{ 0.0f, 0.0f, 0.0f, 0.0f };

		VkRenderPassBeginInfo rinfo;

		rinfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rinfo.pNext = nullptr;
		rinfo.renderArea.offset = { 0, 0 };
		rinfo.renderArea.extent = Context::GetSwapchainExtent();
		rinfo.renderPass = pipeline.GetRenderPass();
		rinfo.framebuffer = pipeline.GetFramebuffer(i);
		rinfo.clearValueCount = 1;
		rinfo.pClearValues = &gay;
		
		vkCmdBeginRenderPass(cmd[i], &rinfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cmd[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());

		uint64 gayOffset = 0;

		vkCmdBindVertexBuffers(cmd[i], 0, 1, &vbo.GetBuffer(), &gayOffset);

		vkCmdBindIndexBuffer(cmd[i], ibo.GetBuffer(), 0, ibo.GetFormat());

		vkCmdDrawIndexed(cmd[i], ibo.GetCount(), 1, 0, 0, 0);

		vkCmdEndRenderPass(cmd[i]);

		vkEndCommandBuffer(cmd[i]);
	}

	unsigned int shit2 = clock();
	unsigned int dankFps = 0;
	while (window.IsOpen()) {
		uint32 imageIndex;

		VK(vkAcquireNextImageKHR(Context::GetDevice(), Context::GetSwapchain(), ~0L, Context::GetImageSemaphore(), 0, &imageIndex));

		VkSubmitInfo info;

		VkPipelineStageFlags shit[]{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = nullptr;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &cmd[imageIndex];
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &Context::GetRenderSemaphore();
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &Context::GetImageSemaphore();
		info.pWaitDstStageMask = shit;

		VK(vkQueueSubmit(Context::GetGraphicsQueue(), 1, &info, 0));

		VkPresentInfoKHR pinfo;

		pinfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		pinfo.pNext = nullptr;
		pinfo.swapchainCount = 1;
		pinfo.pSwapchains = &Context::GetSwapchain();
		pinfo.pImageIndices = &imageIndex;
		pinfo.waitSemaphoreCount = 1;
		pinfo.pWaitSemaphores = &Context::GetRenderSemaphore();
		pinfo.pResults = nullptr;

		VK(vkQueuePresentKHR(Context::GetPresentQueue(), &pinfo));

		window.Update();

		dankFps++;

		if (clock() - shit2 > 1000) {
			shit2 = clock();
			Log::Info("%u", dankFps);
			dankFps = 0;
		}
	
	}

}