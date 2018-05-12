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
/*
const char* vs = "#version 450\n"
""
"#extension GL_ARB_separate_shader_objects : enable\n"
""
"layout(location = 0) in vec3 position;	\n"
"layout(location = 1) in vec4 colors;	\n"
"layout(location = 2) in vec2 texCoords;\n"
""
"out gl_PerVertex{\n"
"	vec4 gl_Position;\n"
"};\n"
""
"layout(location = 0) out vec4 color;\n"
"layout(location = 1) out vec2 texCoord;\n"
""
"layout(binding = 0) uniform UniformBuffer {\n"
"	mat4 matrix;\n"
"};\n"
""
"void main() {\n"
"	gl_Position = matrix * vec4(position.x, -position.y, position.z, 1.0);\n"
""
"	color = colors;\n"
"	texCoord = texCoords;\n"
"}\n"
" ";

const char* ps = "#version 450\n"
""
"#extension GL_ARB_separate_shader_objects : enable\n"
"\n"
"layout(location = 0) out vec4 Color;\n"
""
"layout(location = 0) in vec4 color;\n"
"layout(location = 1) in vec2 texCoord;\n"
""
"layout(binding = 1) uniform sampler2D tex;\n"
""
"void main() {\n"
"	Color = texture(tex, texCoord);\n"
"}\n"
" ";*/

int main() {

	Log::AddDevice(new LogDeviceConsole());

	Factory::CreateFactory();

	WindowCreateInfo winfo;

	winfo.graphicsAdapter = Factory::GetAdapters()[0];
	winfo.outputWindow = nullptr;
	winfo.width = 1280;
	winfo.height = 720;
	winfo.title = "Dank Title";

	Window* window = Window::Create(&winfo);

	BlendInfo blendInfo = { false, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add, ColorComponentFlag::All };
	DepthStencilInfo depthInfo = { };
	//Shader shader(vs, ps, "", true);
	Shader shader("./res/vert.spv", "./res/frag.spv", "");

	Texture2D texture("./res/cube.fdf");
	Texture2D texture2("./res/bricks.fdf");
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
	info.numBlends = 1;
	info.blends = &blendInfo; 
	info.shader = &shader;
	info.numInputLayouts = 1;
	info.shaderInputLayouts = &inputLayout; 

	info.depthStencilInfo = depthInfo;

	List<DescriptorSetBinding> elements;

	elements.Push_back({ DescriptorType::Uniform, 0, sizeof(mat4), 1, ShaderTypeVertex });
	elements.Push_back({ DescriptorType::TextureSampler, 1, 0, 1, ShaderTypePixel });

	DescriptorSetLayout layout(elements, 2);

	PipelineLayout pipelineLayout(&layout);

	DescriptorSet* set = layout.AllocateDescriptorSet();
	DescriptorSet* set2 = layout.AllocateDescriptorSet();

	set->SetTexture(1, &texture, &sampler);
	set2->SetTexture(1, &texture2, &sampler);

	RenderSubPassInfo pass;

	pass.colorAttachments[0] = FD_SWAPCHAIN_ATTACHMENT_INDEX;
	pass.colorAttachments[1] = FD_NO_ATTACHMENT;
	pass.inputAttachments[0] = FD_NO_ATTACHMENT;

	RenderPassInfo passInfo;

	passInfo.depthAttachment = FD_NO_ATTACHMENT;

	passInfo.subpasses.Push_back(pass);

	RenderPass renderPass(&passInfo);

	Pipeline pipeline(&info, &renderPass, 0, &pipelineLayout);

	Vertex vertices[3];
	
	vertices[0].position = vec3(0, 1, 0);
	vertices[0].color = vec4(0, 1, 1, 1);
	vertices[0].texCoords = vec2(0.5f, 0.0f); 
	
	vertices[1].position = vec3(1, -1, 0);
	vertices[1].color = vec4(1, 1, 0, 1); 
	vertices[1].texCoords = vec2(1.0f, 1.0f);
	
	vertices[2].position = vec3(-1, -1, 0);
	vertices[2].color = vec4(1, 0, 1, 1);
	vertices[2].texCoords = vec2(0.0f, 1.0f);

	uint32 indices[]{ 0, 1, 2 };

	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, 3);

	CommandBufferArray cmd = Context::GetCommandBuffers();
	
	cmd.Begin(CommandBufferUsage::Simultaneous);
	cmd.BindPipeline(&pipeline);
	cmd.BeginRenderPass(&renderPass);
	cmd.BindDescriptorSet(&pipelineLayout, 0, set);
	 
	cmd.Bind(&vbo);
	cmd.Bind(&ibo);
	 
	cmd.DrawIndexed(ibo.GetCount());
		
	cmd.End();
	  
	unsigned int shit2 = clock();
 	unsigned int dankFps = 0;
	float aa = 0; 
	while (window->IsOpen()) {
		aa += 0.005f;

		vec3 tmp(0, 0, aa);
		
		mat4 m = mat4::Perspective(1280.0f / 720.0f, 85.0f, 0.01f, 100.0f) * mat4::Translate(vec3(0, 0, 2)) * mat4::Rotate(tmp);

		set->UpdateUniform(0, &m, sizeof(mat4));
		set2->UpdateUniform(0, &m, sizeof(mat4));

		/*cmd.Begin(CommandBufferUsage::Simultaneous);
		cmd.BindPipeline(&pipeline);
		cmd.BeginRenderPass(&renderPass);
		cmd.BindDescriptorSet(&pipelineLayout, 0, set);

		cmd.Bind(&vbo);
		cmd.Bind(&ibo);

		cmd.DrawIndexed(ibo.GetCount());

		cmd.End();*/

		Context::Present(&cmd);

		window->Update(); 

		dankFps++;
		
		if (clock() - shit2 > 1000) {
			shit2 = clock();
			Log::Info("%u", dankFps);
			dankFps = 0;
		}
	
	}

}