#pragma once
#include <vector>
#include <memory>
#include "../Core/Base.h"

namespace Engine {

	class SceneObject : public std::enable_shared_from_this<SceneObject>
	{
	public:
		ENGINE_API static std::shared_ptr<SceneObject> Create();

		ENGINE_API std::vector<std::shared_ptr<SceneObject>> GetChildren() { return this->children; }
		ENGINE_API std::weak_ptr<SceneObject> GetParent() { return this->parent; }

		ENGINE_API void AddChild(std::shared_ptr<SceneObject> newChild);

	private:
		SceneObject();

		std::weak_ptr<SceneObject> parent;
		std::vector<std::shared_ptr<SceneObject>> children;
	};

}