#include "GlobalEngine.h"

#include "../Resource/Loaders/BitmapLoader.h"
#include "../Resource/Loaders/GlslShaderLoader.h"
#include "../Resource/Loaders/ObjLoader.h"


namespace Engine
{
    void GlobalEngine::initialize(std::unique_ptr<Application> injected_app, MainLoop main_loop)
    {
        auto engine = std::make_unique<GlobalEngine>(GlobalEngine());

        engine->m_application = std::move(injected_app);
        engine->m_mainLoop = main_loop;

        engine->m_resourceManager = ResourceManager();
        engine->m_resourceManager.registerLoader(std::make_unique<GlslShaderLoader>());
        engine->m_resourceManager.registerLoader(std::make_unique<BitmapLoader>());
        engine->m_resourceManager.registerLoader(std::make_unique<ObjLoader>());

        engine->m_windowManager = WindowManager();

        engine->m_sceneManager = SceneManager();

        m_globalInstance = std::move(engine);
        m_globalInstance->m_application->initialize();

        m_globalInstance->m_mainLoop.run();
    }

    void GlobalEngine::shutdown()
    {
        m_globalInstance->m_mainLoop.stop();
    }

    GlobalEngine& GlobalEngine::get()
    {
        return *m_globalInstance;
    }

    ResourceManager& GlobalEngine::getResourceManager()
    {
        return this->m_resourceManager;
    }

    WindowManager& GlobalEngine::getWindowManager()
    {
        return this->m_windowManager;
    }

    SceneManager& GlobalEngine::getSceneManager()
    {
        return this->m_sceneManager;
    }

    void GlobalEngine::tick()
    {
        this->m_windowManager.tick();
        this->m_application->tickInternal();
        this->m_application->tick();
    }

    std::unique_ptr<GlobalEngine> GlobalEngine::m_globalInstance = nullptr;
}
