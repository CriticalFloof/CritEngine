#include "SceneObject.h"

namespace Engine {

	SceneObject::SceneObject()
	{
	}

	std::shared_ptr<SceneObject> SceneObject::Create()
	{
		return std::shared_ptr<SceneObject>(new SceneObject());
	}

	void SceneObject::AddChild(std::shared_ptr<SceneObject> newChild)
	{
		this->children.push_back(newChild);
		newChild->parent = this->shared_from_this();
	}

}