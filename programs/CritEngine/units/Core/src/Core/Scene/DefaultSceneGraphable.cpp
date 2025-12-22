#include "DefaultSceneGraphable.h"
#include <algorithm>

namespace Engine {

	std::weak_ptr<ISceneGraphable> DefaultSceneGraphable::GetParent()
	{
		return this->parent;
	}

	void DefaultSceneGraphable::SetParent(std::weak_ptr<ISceneGraphable> newParent)
	{
		this->parent = newParent;
	}

	std::vector<std::shared_ptr<ISceneGraphable>> DefaultSceneGraphable::GetChildren()
	{
		return this->children;
	}

	std::shared_ptr<ISceneGraphable> DefaultSceneGraphable::GetChildByIndex(size_t index)
	{
		return this->children[index];
	}

	/*
		Attempts to return the child matching by name.
		Children without INamed are ignored.
	*/
	std::shared_ptr<ISceneGraphable> DefaultSceneGraphable::GetChildByName(std::string name)
	{
		return nullptr;
	}

	void DefaultSceneGraphable::AddChild(std::shared_ptr<ISceneGraphable> newChild)
	{
		struct Accessor : public ISceneGraphable
		{
			using ISceneGraphable::SetParent;
		};

		this->children.push_back(newChild);
		std::shared_ptr<Accessor> exposed = std::static_pointer_cast<Accessor>(newChild);
		exposed->SetParent(shared_from_this());
	}

	void DefaultSceneGraphable::RemoveChild(std::shared_ptr<ISceneGraphable> targetChild)
	{
		struct Accessor : public ISceneGraphable
		{
			using ISceneGraphable::SetParent;
		};

		for(size_t i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] == targetChild)
			{
				std::shared_ptr<Accessor> exposed = std::static_pointer_cast<Accessor>(this->children[i]);
				exposed->SetParent(std::weak_ptr<ISceneGraphable>());
				std::iter_swap(this->children.begin() + i, this->children.end() - 1);
				this->children.pop_back();
				return;
			}
		}
	}

}