#include "SceneManager.h"

namespace Engine
{
    SceneManager::SceneManager()
        : m_baseScene(std::make_shared<Scene>())
    {
    }
}
