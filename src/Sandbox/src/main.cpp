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
#include <graphics/texture/sampler.h>
#include <graphics/texture/texture.h>
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
using namespace texture;

struct Vertex {
	vec3 position;
	vec4 color;
	vec2 texCoords;
};

int main() {

	Log::AddDevice(new LogDeviceConsole());

	Factory::CreateFactory();

	WindowCreateInfo winfo;

	winfo.graphicsAdapter = Factory::GetAdapters()[0];
	winfo.outputWindow = nullptr;
	winfo.width = 1280;
	winfo.height = 720;
	winfo.refreshRate = 60;
	winfo.title = "Dank Title";

	Window* window = Window::Create(&winfo);

	ViewportInfo viewInfo = { 0, 0, winfo.width, winfo.height, 0.0f, 1.0f };
	ScissorInfo scissorInfo = { 0, 0, viewInfo.width, viewInfo.height };
	BlendInfo blendInfo = { false, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add, ColorComponentFlag::All };
	DepthStencilInfo depthInfo = { };
	Shader shader("./res/vert.spv", "./res/frag.spv", "");

	Texture2D texture("./res/cube.fdf");
	Sampler sampler(SamplerFilter::Linear, SamplerFilter::Linear, SamplerAddressMode::Repeat, SamplerAddressMode::Repeat, SamplerAddressMode::Repeat, true, 16.0f, SamplerBorderColor::Black, true);

	BufferLayout inputLayout(0, BufferInputRate::PerVertex);

	inputLayout.Push<vec3>("POSITION");
	inputLayout.Push<vec4>("COLOR"); 
	inputLayout.Push<vec2>("TEXCOORDS");

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

	info.depthStencilInfo = depthInfo;

	PipelineLayout layout;

	List<PipelineLayoutElement> elements;

	elements.Push_back({ DescriptorType::Uniform, 0, sizeof(mat4), 1, ShaderTypeVertex });
	elements.Push_back({ DescriptorType::TextureSampler, 1, 0, 1, ShaderTypePixel });

	layout.AddSet(elements);

	layout.CreateLayout();

	layout.SetTexture(0, 1, &texture, &sampler);
	 
	RenderSubPassInfo pass;

	pass.colorAttachments[0] = FD_SWAPCHAIN_ATTACHMENT_INDEX;
	pass.colorAttachments[1] = FD_NO_ATTACHMENT;
	pass.inputAttachments[0] = FD_NO_ATTACHMENT;

	RenderPassInfo passInfo;

	passInfo.depthAttachment = FD_NO_ATTACHMENT;

	passInfo.subpasses.Push_back(pass);

	RenderPass renderPass(&passInfo);

	Pipeline pipeline(&info, &renderPass, &layout);

	Vertex vertices[3];
	
	vertices[0].position = vec3(0, 1, 4);
	vertices[0].color = vec4(1, 1, 1, 1);
	vertices[0].texCoords = vec2(0.5f, 0.0f); 
	
	vertices[1].position = vec3(1, -1, 4);
	vertices[1].color = vec4(1, 1, 1, 1); 
	vertices[1].texCoords = vec2(1.0f, 1.0f);
	
	vertices[2].position = vec3(-1, -1, 4);
	vertices[2].color = vec4(1, 1, 1, 1);
	vertices[2].texCoords = vec2(0.0f, 1.0f);

	uint32 indices[]{ 0, 1, 2 };

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, 3); 
	
	Context::BeginCommandBuffers((uint32)VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
	Context::BindPipeline(&pipeline);
	Context::BindRenderPass(&renderPass);
	Context::BindPipelineLayout(&layout);
	 
	Context::Bind(&vbo, 0);
	Context::Bind(&ibo);
	 
	Context::DrawIndexed();
		
	Context::EndCommandBuffers();
	  
	unsigned int shit2 = clock();
 	unsigned int dankFps = 0;
	float aa = 0; 
	while (window->IsOpen()) {
		aa += 0.005f;

		vec3 tmp(0, 0, aa);

		mat4 m = mat4::Perspective(1280.0f / 720.0f, 85.0f, 0.01f, 100.0f) * mat4::Rotate(tmp);

		layout.UpdateUniform(0, 0, &m, sizeof(mat4));

		Context::Present();

		window->Update(); 

		dankFps++;
		
		if (clock() - shit2 > 1000) {
			shit2 = clock();
			Log::Info("%u", dankFps);
			dankFps = 0;
		}
	
	}

}