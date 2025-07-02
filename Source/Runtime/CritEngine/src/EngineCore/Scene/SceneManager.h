#pragma once
#include "Scene.h"

namespace Engine {

	class SceneManager
	{
	public:
		SceneManager();

		ENGINE_API std::shared_ptr<Scene> GetBaseScene() { return this->baseScene; }
		ENGINE_API void SetBaseScene(std::shared_ptr<Scene> scene) { this->baseScene = scene; }

	private:
		std::shared_ptr<Scene> baseScene = nullptr;
	};

}