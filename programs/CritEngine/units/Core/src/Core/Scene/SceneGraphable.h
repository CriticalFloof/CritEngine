#pragma once
#include <memory>
#include <vector>
#include <string>

namespace Engine {

	struct ISceneGraphable
	{
	public:
		virtual std::weak_ptr<ISceneGraphable> GetParent() = 0;
		
		virtual std::vector<std::shared_ptr<ISceneGraphable>> GetChildren() = 0;
		virtual std::shared_ptr<ISceneGraphable> GetChildByIndex(size_t index) = 0;
		virtual std::shared_ptr<ISceneGraphable> GetChildByName(std::string name) = 0;
		
		virtual void AddChild(std::shared_ptr<ISceneGraphable> newChild) = 0;
		virtual void RemoveChild(std::shared_ptr<ISceneGraphable> newChild) = 0;

	protected:
		virtual void SetParent(std::weak_ptr<ISceneGraphable>) = 0;
	};

}