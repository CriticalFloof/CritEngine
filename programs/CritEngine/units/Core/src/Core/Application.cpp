#include "Application.h"
#include "Logging/Logger.h"
#include "Core/GlobalEngine.h"

namespace Engine
{
    Application::Application()
    {
    };

    Application::~Application()
    {
    };

    void Application::initialize()
    {
        // This function gets called after the global engine has been initialized.
    };

    void Application::tickInternal()
    {
        for (Layer* layer : this->m_layerStack)
            layer->onUpdate();
    }

    void Application::pushLayer(Layer* layer)
    {
        this->m_layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer* overlay)
    {
        this->m_layerStack.pushLayer(overlay);
    }

    void Application::tick()
    {
        logInfo("App", "Tick!");
    }
}
