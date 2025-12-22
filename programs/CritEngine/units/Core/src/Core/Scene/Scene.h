#pragma once
#include "SceneGraphable.h"
#include "../Core/Base.h"

namespace Engine {

	class Scene
	{
	public:
		ENGINE_API Scene();

		ENGINE_API std::shared_ptr<ISceneGraphable> GetSceneRoot() { return this->worldRoot; }
		ENGINE_API void SetSceneRoot(std::shared_ptr<ISceneGraphable> scene) { this->worldRoot = scene; }

	private:
		std::shared_ptr<ISceneGraphable> worldRoot = nullptr;

	};

}