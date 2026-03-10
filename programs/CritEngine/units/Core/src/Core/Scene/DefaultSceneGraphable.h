#pragma once
#include "../Core/Base.h"
#include "SceneGraphable.h"

namespace Engine
{
    class DefaultSceneGraphable : public ISceneGraphable, public std::enable_shared_from_this<DefaultSceneGraphable>
    {
    public:
        ENGINE_API std::weak_ptr<ISceneGraphable> getParent() override;

        ENGINE_API std::vector<std::shared_ptr<ISceneGraphable>> getChildren() override;
        ENGINE_API std::shared_ptr<ISceneGraphable> getChildByIndex(size_t index) override;
        ENGINE_API std::shared_ptr<ISceneGraphable> getChildByName(std::string name) override;

        ENGINE_API void addChild(std::shared_ptr<ISceneGraphable> new_child) override;
        ENGINE_API void removeChild(std::shared_ptr<ISceneGraphable> new_child) override;

    protected:
        ENGINE_API void setParent(std::weak_ptr<ISceneGraphable>) override;

    private:
        std::weak_ptr<ISceneGraphable> m_parent;
        std::vector<std::shared_ptr<ISceneGraphable>> m_children;
    };
}
