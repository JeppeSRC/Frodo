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
using namespace event;

struct Vertex {
	vec3 position;
	vec4 color;
	vec2 texCoords;
};

class TestApp : public Application, public EventListener {
public:
	Pipeline* pipeline;
	RenderPass* renderPass;
	PipelineLayout* pipelineLayout;
	DescriptorSetLayout* layout;
	DescriptorSet* set;
	DescriptorSet* set2;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	CommandBufferArray* cmd;
	CommandBufferArray* cmd2;

	Shader* shader;

	uint32 fps;

	TestApp() : Application("TestApp"), EventListener(EventMouse) { }

	~TestApp() {
		delete shader;
		delete ibo;
		delete vbo;
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
		shader = new Shader("./res/vert.spv", "./res/frag.spv", "");

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
		
		elements.Push_back({ DescriptorType::Uniform, 0, sizeof(mat4) * 2, 1, ShaderTypeVertex });
		elements.Push_back({ DescriptorType::TextureSampler, 1, 0, 1, ShaderTypePixel });

		layout = new DescriptorSetLayout(elements, 2);
		pipelineLayout = new PipelineLayout(layout);

		set = layout->AllocateDescriptorSet();
		set2 = layout->AllocateDescriptorSet();

		renderPass = new RenderPass(Format::D32);

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

		mat4 projection = mat4::Perspective((float)window->GetWidth() / (float)window->GetHeight(), 85.0f, 0.01f, 100.0f);

		set->UpdateUniform(0, &projection, sizeof(mat4));
		set2->UpdateUniform(0, &projection, sizeof(mat4));

		cmd = Context::GetPrimaryCommandBuffer();
	}

	float aa = 0;

	vec3 position;

	bool OnMouseEventMove(const math::vec2i& absolute, const math::vec2i& relative) override {
		float posX = (((float)absolute.x / (float)window->GetWidth()) * 2.0f - 1.0f) * 3.5;
		float posY = (((float)absolute.y / (float)window->GetHeight()) * 2.0f - 1.0f) * 2;

		position.x = posX;
		position.y = posY;
		position.z = 2;

		return true;
	}

	void OnUpdate(float delta) override {
		aa += 50.0f * delta;

		vec3 tmp(0, 0, aa);

		mat4 projection = mat4::Perspective((float)window->GetWidth() / (float)window->GetHeight(), 85.0f, 0.01f, 100.0f);

		mat4 m = mat4::Translate(position) * mat4::Rotate(tmp);
		mat4 m2 = mat4::Translate(vec3(2, 0, 2.001)) * mat4::Rotate(-tmp);

		set->UpdateUniform(0, &projection, sizeof(mat4));
		set2->UpdateUniform(0, &projection, sizeof(mat4));

		set->UpdateUniform(0, &m2, sizeof(mat4), sizeof(mat4));
		set2->UpdateUniform(0, &m, sizeof(mat4), sizeof(mat4));
	}

	void OnRender() override {
		cmd->Begin(CommandBufferUsage::Simultaneous, true);
		cmd->BeginRenderPass(renderPass);
		cmd->BindPipeline(pipeline);
		
		cmd->Bind(vbo);
		cmd->Bind(ibo);

		cmd->BindDescriptorSet(pipelineLayout, 0, set);
		cmd->DrawIndexed(ibo->GetCount());

		cmd->BindDescriptorSet(pipelineLayout, 0, set2);
		cmd->DrawIndexed(ibo->GetCount());

		cmd->EndRenderPass();
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
}