#pragma once

#include "../Resource/ResourceManager.h"
#include "../Window/WindowManager.h"
#include "../Application.h"
#include "../Scene/SceneManager.h"
#include "MainLoop.h"


namespace Engine {

	class GlobalEngine
	{
	public:
		ENGINE_API static GlobalEngine& Get();
		ENGINE_API static void Initialize(std::unique_ptr<Application> injectedApp = std::make_unique<Application>(), MainLoop mainLoop = MainLoop());
		ENGINE_API static void Shutdown();

		ENGINE_API ResourceManager& GetResourceManager();
		ENGINE_API WindowManager& GetWindowManager();
		ENGINE_API SceneManager& GetSceneManager();

		void Tick();

	private:
		GlobalEngine();

		static std::unique_ptr<GlobalEngine> globalInstance;

		ResourceManager resourceManager;
		WindowManager windowManager;
		SceneManager sceneManager;
		std::unique_ptr<Application> application = nullptr;
		MainLoop mainLoop;
	};

}