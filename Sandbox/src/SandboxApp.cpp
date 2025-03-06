#pragma once

#include <cstdio>
#include <functional>
#include <iostream>
#include <sstream>

#include <EngineCore/Layer.h>
#include <EngineCore/Application.h>
#include <EngineCore/Core/MainLoop.h>
#include <EngineCore/Logging/Logger.h>
#include <EngineCore/Event/Event.h>
#include <EngineCore/Event/EventBus.h>
#include <EngineCore/Window/Input.h>
#include <EngineCore/Entry.h>
#include <EngineCore/Graphics/Renderer.h>

std::string vertexShaderSource = R"(
	#version 460 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	out vec4 vColor;

	void main()
	{
		
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		vColor = aColor;
	}
)";

std::string fragmentShaderSource = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vColor;
	}
)";

class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {

	}

	void OnUpdate() override {
		LogInfo("ExampleLayer", "Update");
	}

};

class Sandbox : public Engine::Application
{

public:

	std::unique_ptr<Engine::EventBus> bus;
	std::shared_ptr<Engine::Event<int>> intEmitter;
	std::shared_ptr<Engine::Event<>> voidEmitter;

	Sandbox() {
		PushLayer(new LayerTest());

		this->intEmitter.reset(new Engine::Event<int>());
		this->voidEmitter.reset(new Engine::Event<>());

		
		this->bus = std::make_unique<Engine::EventBus>();
		this->bus->AddEvent("emit1", voidEmitter);
		this->bus->AddEvent<int>("emit2", intEmitter);

		this->bus->AddListener("emit1", { []() {
			LogInfo("voidEmitter", "Void emitter emitted"); 
		}});

		this->bus->AddListener<int>("emit2", { [](int i) {
			LogInfo("intEmitter", std::to_string(i));
		}});

		this->bus->Emit("emit2", 600);
		this->bus->Emit("emit2", 1000);
		this->bus->Emit("emit1");
		this->bus->Emit("emit2", 1);
		this->bus->RemoveListener<int>("emit2", { [](int i) {
			LogInfo("intEmitter", std::to_string(i));
			}});
		this->bus->Emit("emit2", 6);
		

		this->shader.reset(new Engine::Shader(vertexShaderSource, fragmentShaderSource));

		this->triangleVA = Engine::VertexArray::Create();

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> triangleVB = Engine::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));
		triangleVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float4, "aColor"},
							  });
		triangleVA->AddVertexBuffer(triangleVB);

		uint32_t triangleIndices[3] = { 0, 1, 2 };
		std::shared_ptr<Engine::IndexBuffer> triangleIB = Engine::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));
		triangleVA->SetIndexBuffer(triangleIB);

		this->squareVA = Engine::VertexArray::Create();

		float squareVertices[4 * 7] = {
			-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> squareVB = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float4, "aColor"},
							});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> squareIB = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		squareVA->SetIndexBuffer(squareIB);

	}


	void Tick() override
	{
		// We don't have access to GLFW's key enums, so we may need to import them (?)
		if (this->GetWindow().GetInput().GetKeyDown(66))
		{ // If B is pressed
			 LogWarning("Input", "B pressed!");
		}
		if (this->GetWindow().GetInput().GetKeyUp(66))
		{ // If B is released
			LogWarning("Input", "B released!");
		}
		if (this->GetWindow().GetInput().GetKeyJustPressed(67))
		{
			LogWarning("Input", "C just pressed!");
		}

		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene();

		this->shader->Bind();
		Engine::Renderer::Submit(this->squareVA);
		Engine::Renderer::Submit(this->triangleVA);


		Engine::Renderer::EndScene();

		LogInfo("Sandbox", "Tick!");
	}

	~Sandbox()
	{
		LogWarning("Sandbox", "Destroyed!");
	}
private:
	std::shared_ptr<Engine::Shader> shader;
	std::shared_ptr<Engine::VertexArray> triangleVA;
	std::shared_ptr<Engine::VertexArray> squareVA;
};

std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};