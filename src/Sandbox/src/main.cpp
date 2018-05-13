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

#include <core/app/application.h>

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
using namespace app;

struct Vertex {
	vec3 position;
	vec4 color;
	vec2 texCoords;
};

class TestApp : public Application {
public:
	Pipeline* pipeline;
	RenderPass* renderPass;
	PipelineLayout* pipelineLayout;
	DescriptorSetLayout* layout;
	DescriptorSet* set;
	DescriptorSet* set2;
	Depthbuffer* depthBuffer;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	CommandBufferArray* cmd;

	Texture2D* texture;
	Texture2D* texture2;
	Sampler* sampler;

	Shader* shader;

	uint32 fps;

	TestApp() : Application("TestApp") { }

	~TestApp() {
		delete shader;
		delete texture;
		delete texture2;
		delete sampler;
		delete ibo;
		delete vbo;
		delete depthBuffer;
		delete layout;
		delete pipelineLayout;
		delete renderPass;
		delete pipeline;
	}

	void OnWindowCreate(WindowCreateInfo* const winfo) override {
		winfo->graphicsAdapter = Factory::GetAdapters()[0];
		winfo->outputWindow = nullptr;
		winfo->width = 1280;
		winfo->height = 720;
		winfo->title = "Dank Title";
	}

	void OnInit() override {
		BlendInfo blendInfo = { false, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add, ColorComponentFlag::All };
		DepthStencilInfo depthInfo = { true, true, ComparisonFunc::LessEqual, false };
		//Shader shader(vs, ps, "", true);
		shader = new Shader("./res/vert.spv", "./res/frag.spv", "");

		texture = new Texture2D("./res/cube.fdf");
		texture2 = new Texture2D("./res/bricks.fdf");

		sampler = new Sampler(SamplerFilter::Linear, SamplerFilter::Linear, SamplerAddressMode::Repeat, SamplerAddressMode::Repeat, SamplerAddressMode::Repeat, true, 16.0f, SamplerBorderColor::Black, true);

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
		info.shader = shader;
		info.numInputLayouts = 1;
		info.shaderInputLayouts = &inputLayout;

		info.depthStencilInfo = depthInfo;

		List<DescriptorSetBinding> elements;

		elements.Push_back({ DescriptorType::Uniform, 0, sizeof(mat4), 1, ShaderTypeVertex });
		elements.Push_back({ DescriptorType::TextureSampler, 1, 0, 1, ShaderTypePixel });

		layout = new DescriptorSetLayout(elements, 2);
		pipelineLayout = new PipelineLayout(layout);


		set = layout->AllocateDescriptorSet();
		set2 = layout->AllocateDescriptorSet();

		set->SetTexture(1, texture, sampler);
		set2->SetTexture(1, texture2, sampler);

		RenderSubPassInfo pass;

		pass.colorAttachments[0] = FD_SWAPCHAIN_ATTACHMENT_INDEX;
		pass.colorAttachments[1] = FD_NO_ATTACHMENT;
		pass.inputAttachments[0] = FD_NO_ATTACHMENT;

		RenderPassInfo passInfo;

		depthBuffer = new Depthbuffer(1280, 720, Format::D16);

		passInfo.clearColor = vec4(0, 0, 0, 0);
		passInfo.depthClearValue = 1.0f;

		passInfo.framebuffers.Push_back(depthBuffer);

		passInfo.depthAttachment = 0;

		passInfo.subpasses.Push_back(pass);

		renderPass = new RenderPass(&passInfo);

		pipeline = new Pipeline(&info, renderPass, 0, pipelineLayout);

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

		vbo = new VertexBuffer(vertices, sizeof(vertices));
		ibo = new IndexBuffer(indices, 3);

		cmd = Context::GetCommandBuffers();
	}

	float aa = 0;

	void OnUpdate(float delta) override {
		aa += 50.0f * delta;

		vec3 tmp(0, 0, aa);

		mat4 m = mat4::Perspective((float)window->GetWidth() / (float)window->GetHeight(), 85.0f, 0.01f, 100.0f) * mat4::Translate(vec3(-2, 0, 2)) * mat4::Rotate(tmp);
		mat4 m2 = mat4::Perspective((float)window->GetWidth() / (float)window->GetHeight(), 85.0f, 0.01f, 100.0f) * mat4::Translate(vec3(2, 0, 2)) * mat4::Rotate(-tmp);

		set->UpdateUniform(0, &m, sizeof(mat4));
		set2->UpdateUniform(0, &m2, sizeof(mat4));
	}

	void OnRender() override {
		cmd->Begin(CommandBufferUsage::Simultaneous);
		cmd->BindPipeline(pipeline);
		cmd->BeginRenderPass(renderPass);

		cmd->Bind(vbo);
		cmd->Bind(ibo);

		cmd->BindDescriptorSet(pipelineLayout, 0, set);
		cmd->DrawIndexed(ibo->GetCount());

		cmd->BindDescriptorSet(pipelineLayout, 0, set2);
		cmd->DrawIndexed(ibo->GetCount());

		cmd->End();

		fps++;
	}

	void OnTick() {
		Log::Info("FPS: %u", fps);
		fps = 0;
	}
};

int main() {
	TestApp app;

	app.Start();

	return 0;
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
	DepthStencilInfo depthInfo = { true, true, ComparisonFunc::LessEqual, false };
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

	Depthbuffer depthBuffer(1280, 720, Format::D16);

	passInfo.clearColor = vec4(0, 0, 0, 0);
	passInfo.depthClearValue = 1.0f;

	passInfo.framebuffers.Push_back(&depthBuffer);

	passInfo.depthAttachment = 0;
	
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

	CommandBufferArray* cmd = Context::GetCommandBuffers();
	
	cmd->Begin(CommandBufferUsage::Simultaneous);
	cmd->BindPipeline(&pipeline);
	cmd->BeginRenderPass(&renderPass);
	cmd->BindDescriptorSet(&pipelineLayout, 0, set);
	   
	cmd->Bind(&vbo);
	cmd->Bind(&ibo);
	   
	cmd->DrawIndexed(ibo.GetCount());
		
	cmd->End();
	  
	unsigned int shit2 = clock();
 	unsigned int dankFps = 0;
	float aa = 0; 
	while (window->IsOpen()) {
		aa += 0.005f;

		vec3 tmp(0, 0, aa);
		
		mat4 m = mat4::Perspective((float)window->GetWidth() / (float)window->GetHeight(), 85.0f, 0.01f, 100.0f) * mat4::Translate(vec3(-2, 0, 2)) * mat4::Rotate(tmp);
		mat4 m2 = mat4::Perspective((float)window->GetWidth() / (float)window->GetHeight(), 85.0f, 0.01f, 100.0f) * mat4::Translate(vec3(2, 0, 2)) * mat4::Rotate(-tmp);

		set->UpdateUniform(0, &m, sizeof(mat4));
		set2->UpdateUniform(0, &m2, sizeof(mat4));

		cmd->Begin(CommandBufferUsage::Simultaneous);
		cmd->BindPipeline(&pipeline);
		cmd->BeginRenderPass(&renderPass);

		cmd->Bind(&vbo);
		cmd->Bind(&ibo);

		cmd->BindDescriptorSet(&pipelineLayout, 0, set);
		cmd->DrawIndexed(ibo.GetCount());

		cmd->BindDescriptorSet(&pipelineLayout, 0, set2);
		cmd->DrawIndexed(ibo.GetCount());

		cmd->End();

		Context::Present(cmd);

		window->Update(); 

		dankFps++;
		
		if (clock() - shit2 > 1000) {
			shit2 = clock();
			Log::Info("%u", dankFps);
			dankFps = 0;
		}
	
	}

}