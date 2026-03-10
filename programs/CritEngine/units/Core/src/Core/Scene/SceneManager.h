#pragma once
#include "Scene.h"

namespace Engine
{
    class SceneManager
    {
    public:
        SceneManager();

        ENGINE_API std::shared_ptr<Scene> getBaseScene() { return this->m_baseScene; }
        ENGINE_API void setBaseScene(std::shared_ptr<Scene> scene) { this->m_baseScene = scene; }

    private:
        std::shared_ptr<Scene> m_baseScene = nullptr;
    };
}
