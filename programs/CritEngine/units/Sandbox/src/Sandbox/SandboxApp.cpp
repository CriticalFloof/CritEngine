#include <functional>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <memory>

#include <Core/Layer.h>
#include <Core/Application.h>
#include <Core/Core/GlobalEngine.h>
#include <Core/Logging/Logger.h>
#include <Core/Event/Event.h>
#include <Core/Input/Input.h>
#include <Core/Math/Vector2.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/Camera.h>
#include <Core/Graphics/PIL/Texture.h>
#include <Core/Resource/Resource.h>
#include <Core/ECS/Context.h>
#include <Core/ECS/Components/Spatial.h>
#include <Core/Profiler/Profiler.h>
#include <Core/Entry.h>

#include <Core/Threading/ThreadingHelpers.h>
#include <Core/Resource/Loaders/GLSLShaderLoader.h>
#include <Core/Scene/Actor.h>
#include <Core/Serialization/Serializer.h>

#include <imgui.h>
#include <Core/Graphics/Material.h>
#include <Core/Graphics/Model.h>

const std::filesystem::path ROOT_ASSET_PATH = ((std::filesystem::path)(__FILE__)).parent_path().parent_path().parent_path() / "data"; // TODO: This is temporary, the engine should provide easy to use "virtual" file system capabilities.

class LayerTest : public Engine::Layer {

public:

	LayerTest() : Layer("ExampleLayer") {
	   
	}

	void OnUpdate() override {
		//LogInfo("ExampleLayer", "Update");
	}

};

class Wrapper
{
public:
	Engine::Actor actor;
	double x = 0.1;
	double y = 0.7;
	std::array<uint32_t, 8> arr = {1};
};

REFL_TYPE(Wrapper, bases<>)
	REFL_FIELD(actor, refl::attr::usage::member())
	REFL_FIELD(x, refl::attr::usage::member())
	REFL_FIELD(y, refl::attr::usage::member())
	REFL_FIELD(arr, refl::attr::usage::member())
REFL_END


class Sandbox : public Engine::Application
{

public:
	// Components will be just data structures that inherit from the Component type. This is an example

	// Systems essentially add underlying behavior to components
	class PhysicsSystem : public ECS::System {
	public:

		void Update() override {

			// Perform logic, make modifications, etc..
			const std::chrono::time_point<std::chrono::high_resolution_clock> curTime = std::chrono::high_resolution_clock::now();
			const std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - this->prevTime);
			for (ECS::EntityID entity : ECS::ContextView<SpatialComponent>(*context))
			{
				SpatialComponent* primitive = this->context->GetComponent<SpatialComponent>(entity);
				
				primitive->rotation = (primitive->rotation * Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, (deltaTime.count() * 10.f), 0))).Normalize();
				primitive->position.y = sinf(std::chrono::duration_cast<std::chrono::milliseconds>(curTime - this->startTime).count() * 0.001f);

			}
			this->prevTime = curTime;
		}

		std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();
	};
  
	void Initialize() override
	{
		CE_PROFILE_FUNC(SandboxInitialization);

		// Scene population

		std::shared_ptr<Engine::Scene> scene = Engine::GlobalEngine::Get().GetSceneManager().GetBaseScene();

		scene->SetSceneRoot(std::make_shared<Engine::Actor>());
		scene->GetSceneRoot()->AddChild(std::make_shared<Engine::Actor>());
		Engine::Actor* actor = static_cast<Engine::Actor*>(scene->GetSceneRoot().get());

		// Serialization Test

		//std::stringstream stream;

		Wrapper e = Wrapper();
		e.actor = *actor;
		e.actor.h = "Hello Serializer";
		e.actor.health = 3;
		e.actor.mana = 4;
		e.x = 777.7;
		e.y = 888.8;
		
		std::string json = Serialization::ToJson(e);
		std::cout << json << '\n';
		auto json_view = std::string_view(json);
		Wrapper ne;
		Serialization::FromJson<Wrapper>(json_view, ne);

		// Window Setup
		this->window = Engine::GlobalEngine::Get().GetWindowManager().CreateWindow(800, 600, "Sandbox");
		std::shared_ptr<Engine::Window> window = this->window.lock();

		// Linking Window Events to functions
		window->GetInput()->OnMouseMove([this](Engine::Vector2(cursorPosition)) { this->MoveCameraLook(cursorPosition); });
		window->GetInput()->OnKeyPressed([this](unsigned int key) { this->MoveCameraPosition(key); });

		// Create A Mock ECS System
		this->physicsSystem = PhysicsSystem();
		this->physicsSystem.SetContext(&this->ECSContext);


		this->player = this->ECSContext.CreateEntity();

		SpatialComponent* primitive = this->ECSContext.Assign<SpatialComponent>(player);

		primitive->velocity.x = .0025f;

		//////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		PushLayer(new LayerTest());

		// Camera setup
		Engine::Quaternion camera_rot = Engine::Quaternion::FromEulerAngles(Engine::Vector3(0.4f, 0.f, 0.f));
		this->camera.reset(new Engine::PerspectiveCamera(30, window->GetAspectRatio(), 0.01f, 100, Engine::Vector3(0, 1.25, 10), camera_rot));

		// Mesh from files setup
		Engine::Resource vertexShaderSource = Engine::Resource("Shader", ROOT_ASSET_PATH / "Shaders/shader.vertshader");
		Engine::Resource fragmentShaderSource = Engine::Resource("Shader", ROOT_ASSET_PATH / "Shaders/shader.fragshader");

		Engine::Resource textureHandle = Engine::Resource("Image", ROOT_ASSET_PATH / "Textures/Aegis_Jockey.bmp");
		std::shared_ptr<Engine::Texture> sampleTexture = std::static_pointer_cast<Engine::Texture>(textureHandle.Get());

		std::shared_ptr<Engine::Shader> vertexShader = std::static_pointer_cast<Engine::Shader>(vertexShaderSource.Get());
		std::shared_ptr<Engine::Shader> fragmentShader = std::static_pointer_cast<Engine::Shader>(fragmentShaderSource.Get());

		this->material = Engine::Material::Create(vertexShader, fragmentShader, {sampleTexture});

		Engine::Resource cubeSource = Engine::Resource("Mesh", ROOT_ASSET_PATH / "Meshes/AegisSphere.obj");
		std::shared_ptr<Engine::Mesh> cubeMesh = std::static_pointer_cast<Engine::Mesh>(cubeSource.Get());
		cubeMesh->SetMaterial(this->material);

		this->cubeModel = std::make_shared<Engine::Model>(cubeMesh);

		// Procedural Floor Mesh

		float floorVertices[4 * 9] = {
			-50.0f, 0.0f,-50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 50.0f, 0.0f,-50.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 50.0f, 0.0f, 50.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-50.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Engine::VertexBuffer> floorVB = Engine::VertexBuffer::Create(floorVertices, sizeof(floorVertices));
		floorVB->SetLayout({
			{Engine::ShaderDataType::Float3, "aPos"},
			{Engine::ShaderDataType::Float2, "aTexUV"},
			{Engine::ShaderDataType::Float4, "aColor"},
		});

		uint32_t floorIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine::IndexBuffer> floorIB = Engine::IndexBuffer::Create(floorIndices, sizeof(floorIndices) / sizeof(uint32_t));

		std::shared_ptr<Engine::Mesh> floorMesh = Engine::Mesh::Create();
		floorMesh->AddVertexBuffer(floorVB);
		floorMesh->SetIndexBuffer(floorIB);
		floorMesh->SetMaterial(this->material);

		this->floorModel = std::make_shared<Engine::Model>(floorMesh);
		
		Engine::RenderCommand::SetClearColor({ 0.8, 0.2, 0.8, 1 });
		Engine::RenderCommand::EnableDepthTest(true);
		Engine::RenderCommand::EnableDepthMask(true);
		Engine::RenderCommand::SetDepthTestFunc(Engine::RendererAPI::DepthTestFunction::Less);
	}

	void MoveCameraLook(Engine::Vector2 cursorPosition)
	{
		if (this->prevCursorPos.x == 0 && this->prevCursorPos.y == 0)
		{
			this->prevCursorPos = cursorPosition;
		}
		Engine::Vector2 velocity = (cursorPosition - this->prevCursorPos) / 1000.f;
		
		// Prevent roll by separating yaw and pitch rotations in the multiplication order.
		this->camera->setRotation(Engine::Quaternion::FromEulerAngles(Engine::Vector3(velocity.y, 0, 0)) * this->camera->getRotation() * Engine::Quaternion::FromEulerAngles(Engine::Vector3(0, velocity.x, 0)));
		
		this->prevCursorPos = cursorPosition;
		return;
	}

	void MoveCameraPosition(unsigned int key)
	{
		Engine::Vector3 cameraPosition = this->camera->getPosition();
		Engine::Quaternion cameraRotation = this->camera->getRotation();
		
		Engine::Vector3 forwardVector = cameraRotation.RotateVector(Engine::Vector3(0, 0, -1));
		Engine::Vector3 rightVector = cameraRotation.RotateVector(Engine::Vector3(1, 0, 0));
		if (key == Keys::KeyMap[Keys::W])
		{
			cameraPosition = cameraPosition + (forwardVector * 0.1f);
		}
		if (key == Keys::KeyMap[Keys::A])
		{
			cameraPosition = cameraPosition + (rightVector * -0.1f);
		}
		if (key == Keys::KeyMap[Keys::S])
		{
			cameraPosition = cameraPosition + (forwardVector * -0.1f);
		}
		if (key == Keys::KeyMap[Keys::D])
		{
			cameraPosition = cameraPosition + (rightVector * 0.1f);
		}
		if (key == Keys::KeyMap[Keys::SPACE])
		{
			cameraPosition = cameraPosition + (Engine::Vector3(0, 1, 0) * 0.1f);
		}
		if (key == Keys::KeyMap[Keys::LEFT_SHIFT])
		{
			cameraPosition = cameraPosition + (Engine::Vector3(0, 1, 0) * -0.1f);
		}
		
		
		this->camera->setPosition(cameraPosition);
	}

	void Tick() override
	{	
		// Testing our ECS 
		//CE_PROFILE_FUNC(UpdateLoop);
		std::shared_ptr<Engine::Window> window = this->window.lock();
		
		this->physicsSystem.Update();
		SpatialComponent* transform = this->ECSContext.GetComponent<SpatialComponent>(player);

		this->camera->setAspectRatio(window->GetAspectRatio());

		this->cubeModel->SetProjection(transform->GetMatrix());

		
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(this->camera);

		Engine::Renderer::Submit(this->floorModel);
		Engine::Renderer::Submit(this->cubeModel);
		
		Engine::Renderer::EndScene();
	}

	~Sandbox() override
	{
		LogWarning("Sandbox", "Destroyed!");
	}

private:

	PhysicsSystem physicsSystem;
	ECS::Context ECSContext;
	ECS::EntityID player;

	std::weak_ptr<Engine::Window> window;

	std::shared_ptr<Engine::Material> material;
	std::shared_ptr<Engine::Model> floorModel;
	std::shared_ptr<Engine::Model> cubeModel;
	std::shared_ptr<Engine::PerspectiveCamera> camera;
	Engine::Vector2 prevCursorPos;
};


std::unique_ptr<Engine::Application> CreateApplication()
{
	return std::make_unique<Sandbox>();
};