#include "GlobalEngine.h"

#include "../Core/Reflection/ReflectedKinds/Primitives.h" // Reflection
#include "../Resource/Loaders/BitmapLoader.h"
#include "../Resource/Loaders/GLSLShaderLoader.h"
#include "../Resource/Loaders/ObjLoader.h"


namespace Engine {

	void GlobalEngine::Initialize(std::unique_ptr<Application> injectedApp, MainLoop mainLoop)
	{
		std::unique_ptr<GlobalEngine> engine = std::make_unique<GlobalEngine>(GlobalEngine());

		engine->application = std::move(injectedApp);
		engine->mainLoop = mainLoop;

		engine->resourceManager = ResourceManager();
		engine->resourceManager.RegisterLoader(std::make_unique<GLSLShaderLoader>());
		engine->resourceManager.RegisterLoader(std::make_unique<BitmapLoader>());
		engine->resourceManager.RegisterLoader(std::make_unique<ObjLoader>());

		engine->windowManager = WindowManager();

		engine->sceneManager = SceneManager();

		GlobalEngine::globalInstance = std::move(engine);
		GlobalEngine::globalInstance->application->Initialize();

		GlobalEngine::globalInstance->mainLoop.Run();
	}

	void GlobalEngine::Shutdown()
	{
		GlobalEngine::globalInstance->mainLoop.Stop();
	}

	GlobalEngine& GlobalEngine::Get()
	{
		return *GlobalEngine::globalInstance;
	}

	ResourceManager& GlobalEngine::GetResourceManager()
	{
		return this->resourceManager;
	}

	WindowManager& GlobalEngine::GetWindowManager()
	{
		return this->windowManager;
	}
	
	SceneManager& GlobalEngine::GetSceneManager()
	{
		return this->sceneManager;
	}

	void GlobalEngine::Tick()
	{
		this->windowManager.Tick();
		this->application->TickInternal();
		this->application->Tick();
	}

	GlobalEngine::GlobalEngine()
	{
	}

	std::unique_ptr<GlobalEngine> GlobalEngine::globalInstance = nullptr;
}