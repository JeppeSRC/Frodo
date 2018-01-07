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
	BlendInfo blendInfo = { false, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add, ColorComponentFlag::Red };
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


	while (window.IsOpen()) {

		window.Update();
	}

}