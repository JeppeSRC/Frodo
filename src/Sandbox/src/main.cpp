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
	info.pipelineLayout.elements = new PipelineLayoutElement[3];
	info.pipelineLayout.elements[0].count = 1;
	info.pipelineLayout.elements[0].shaderAccess = ShaderTypePixel;
	info.pipelineLayout.elements[0].type = BufferType::Uniform;
	info.pipelineLayout.elements[0].size = sizeof(vec4);
	info.pipelineLayout.elements[1].count = 1;
	info.pipelineLayout.elements[1].shaderAccess = ShaderTypePixel;
	info.pipelineLayout.elements[1].type = BufferType::Uniform;
	info.pipelineLayout.elements[1].size = sizeof(vec4);
	info.pipelineLayout.elements[2].count = 1;
	info.pipelineLayout.elements[2].shaderAccess = ShaderTypePixel;
	info.pipelineLayout.elements[2].type = BufferType::Uniform;
	info.pipelineLayout.elements[2].size = sizeof(float32);
	info.depthStencilInfo = depthInfo;

	Pipeline pipeline(&info);

	Vertex vertices[3];
	
	vertices[0].position = vec3(0, -1, 0);
	vertices[0].color = vec4(1, 1, 1, 1);
	
	vertices[1].position = vec3(1, 1, 0);
	vertices[1].color = vec4(1, 1, 1, 1); 

	vertices[2].position = vec3(-1, 1, 0);
	vertices[2].color = vec4(1, 1, 1, 1);

	uint32 indices[]{ 0, 1, 2 };

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, 3);

	Context::BeginCommandBuffers();
	Context::BeginRenderPass(&pipeline); 
	
	Context::Bind(&vbo, 0);
	Context::Bind(&ibo);

	Context::DrawIndexed();
	 
	Context::EndCommandBuffers();
	  
	unsigned int shit2 = clock();
 	unsigned int dankFps = 0;
	float aa = 0; 
	while (window.IsOpen()) {

		 
		Context::Present();

		window.Update(); 

		dankFps++;
		
		if (clock() - shit2 > 1000) {
			shit2 = clock();
			Log::Info("%u", dankFps);
			dankFps = 0;
		}
	
	}

}