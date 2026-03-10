#pragma once

#include "../Resource/ResourceManager.h"
#include "../Window/WindowManager.h"
#include "../Application.h"
#include "../Scene/SceneManager.h"
#include "MainLoop.h"


namespace Engine
{
    class GlobalEngine
    {
    public:
        ENGINE_API static GlobalEngine& get();
        ENGINE_API static void initialize(std::unique_ptr<Application> injected_app = std::make_unique<Application>(),
                                          MainLoop main_loop = MainLoop());
        ENGINE_API static void shutdown();

        ENGINE_API ResourceManager& getResourceManager();
        ENGINE_API WindowManager& getWindowManager();
        ENGINE_API SceneManager& getSceneManager();

        void tick();

    private:

        static std::unique_ptr<GlobalEngine> m_globalInstance;

        ResourceManager m_resourceManager;
        WindowManager m_windowManager;
        SceneManager m_sceneManager;
        std::unique_ptr<Application> m_application = nullptr;
        MainLoop m_mainLoop;
    };
}
