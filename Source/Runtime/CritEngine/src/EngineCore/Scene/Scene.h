#pragma once
#include "SceneObject.h"


namespace Engine {

	class Scene
	{
	public:
		ENGINE_API Scene();

		ENGINE_API std::shared_ptr<SceneObject> GetSceneRoot() { return this->worldRoot; }
		ENGINE_API void SetSceneRoot(std::shared_ptr<SceneObject> scene) { this->worldRoot = scene; }

	private:
		std::shared_ptr<SceneObject> worldRoot = nullptr;

	};

}