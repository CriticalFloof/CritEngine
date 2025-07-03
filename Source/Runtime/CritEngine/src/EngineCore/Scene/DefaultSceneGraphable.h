#pragma once
#include "SceneGraphable.h"

namespace Engine {

	class DefaultSceneGraphable : public ISceneGraphable, public std::enable_shared_from_this<DefaultSceneGraphable>
	{
	public:
		virtual std::weak_ptr<ISceneGraphable> GetParent() override;

		virtual std::vector<std::shared_ptr<ISceneGraphable>> GetChildren() override;
		virtual std::shared_ptr<ISceneGraphable> GetChildByIndex(size_t index) override;
		virtual std::shared_ptr<ISceneGraphable> GetChildByName(std::string name) override;

		virtual void AddChild(std::shared_ptr<ISceneGraphable> newChild) override;
		virtual void RemoveChild(std::shared_ptr<ISceneGraphable> newChild) override;

	protected:
		virtual void SetParent(std::weak_ptr<ISceneGraphable>) override;

	private:
		std::weak_ptr<ISceneGraphable> parent;
		std::vector<std::shared_ptr<ISceneGraphable>> children;
	};

}