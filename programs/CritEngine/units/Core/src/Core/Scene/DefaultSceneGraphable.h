#pragma once
#include "../Core/Base.h"
#include "SceneGraphable.h"

namespace Engine {

	class DefaultSceneGraphable : public ISceneGraphable, public std::enable_shared_from_this<DefaultSceneGraphable>
	{
	public:
		ENGINE_API virtual std::weak_ptr<ISceneGraphable> GetParent() override;

		ENGINE_API virtual std::vector<std::shared_ptr<ISceneGraphable>> GetChildren() override;
		ENGINE_API virtual std::shared_ptr<ISceneGraphable> GetChildByIndex(size_t index) override;
		ENGINE_API virtual std::shared_ptr<ISceneGraphable> GetChildByName(std::string name) override;

		ENGINE_API virtual void AddChild(std::shared_ptr<ISceneGraphable> newChild) override;
		ENGINE_API virtual void RemoveChild(std::shared_ptr<ISceneGraphable> newChild) override;

	protected:
		ENGINE_API virtual void SetParent(std::weak_ptr<ISceneGraphable>) override;

	private:
		std::weak_ptr<ISceneGraphable> parent;
		std::vector<std::shared_ptr<ISceneGraphable>> children;
	};

}