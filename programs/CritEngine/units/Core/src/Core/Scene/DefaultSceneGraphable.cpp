#include "DefaultSceneGraphable.h"
#include <algorithm>

namespace Engine
{
    std::weak_ptr<ISceneGraphable> DefaultSceneGraphable::getParent()
    {
        return this->m_parent;
    }

    void DefaultSceneGraphable::setParent(std::weak_ptr<ISceneGraphable> newParent)
    {
        this->m_parent = newParent;
    }

    std::vector<std::shared_ptr<ISceneGraphable>> DefaultSceneGraphable::getChildren()
    {
        return this->m_children;
    }

    std::shared_ptr<ISceneGraphable> DefaultSceneGraphable::getChildByIndex(size_t index)
    {
        return this->m_children[index];
    }

    /*
        Attempts to return the child matching by name.
        Children without INamed are ignored.
    */
    std::shared_ptr<ISceneGraphable> DefaultSceneGraphable::getChildByName(std::string name)
    {
        return nullptr;
    }

    void DefaultSceneGraphable::addChild(std::shared_ptr<ISceneGraphable> new_child)
    {
        struct Accessor : ISceneGraphable
        {
            using ISceneGraphable::setParent;
        };

        this->m_children.push_back(new_child);
        std::shared_ptr<Accessor> exposed = std::static_pointer_cast<Accessor>(new_child);
        exposed->setParent(shared_from_this());
    }

    void DefaultSceneGraphable::removeChild(std::shared_ptr<ISceneGraphable> targetChild)
    {
        struct Accessor : ISceneGraphable
        {
            using ISceneGraphable::setParent;
        };

        for (size_t i = 0; i < this->m_children.size(); i++)
        {
            if (this->m_children[i] == targetChild)
            {
                std::shared_ptr<Accessor> exposed = std::static_pointer_cast<Accessor>(this->m_children[i]);
                exposed->setParent(std::weak_ptr<ISceneGraphable>());
                std::iter_swap(this->m_children.begin() + i, this->m_children.end() - 1);
                this->m_children.pop_back();
                return;
            }
        }
    }
}
