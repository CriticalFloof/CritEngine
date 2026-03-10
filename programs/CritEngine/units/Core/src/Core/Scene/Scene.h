#pragma once
#include "SceneGraphable.h"
#include "../Core/Base.h"

namespace Engine
{
    class Scene
    {
    public:
        ENGINE_API Scene();

        ENGINE_API std::shared_ptr<ISceneGraphable> getSceneRoot() { return m_worldRoot; }
        ENGINE_API void setSceneRoot(std::shared_ptr<ISceneGraphable> scene) { m_worldRoot = scene; }

    private:
        std::shared_ptr<ISceneGraphable> m_worldRoot = nullptr;
    };
}
