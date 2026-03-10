#pragma once
#include <memory>
#include <vector>
#include <string>

namespace Engine
{
    struct ISceneGraphable
    {
        virtual std::weak_ptr<ISceneGraphable> getParent() = 0;

        virtual std::vector<std::shared_ptr<ISceneGraphable>> getChildren() = 0;
        virtual std::shared_ptr<ISceneGraphable> getChildByIndex(size_t index) = 0;
        virtual std::shared_ptr<ISceneGraphable> getChildByName(std::string name) = 0;

        virtual void addChild(std::shared_ptr<ISceneGraphable> new_child) = 0;
        virtual void removeChild(std::shared_ptr<ISceneGraphable> new_child) = 0;

    protected:
        virtual void setParent(std::weak_ptr<ISceneGraphable>) = 0;
    };
}
