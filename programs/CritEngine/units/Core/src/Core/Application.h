#pragma once

#include <memory>
#include "Core/Base.h"
#include "Window/Window.h"
#include "LayerStack.h"
#include "Event/KeyboardEvent.h"
#include "Input/Input.h"

namespace Engine
{
    class Application
    {
    public:
        ENGINE_API Application();
        ENGINE_API virtual ~Application();

        ENGINE_API void virtual initialize();

        ENGINE_API void tickInternal();
        ENGINE_API void virtual tick();

        ENGINE_API void pushLayer(Layer* layer);
        ENGINE_API void pushOverlay(Layer* overlay);

    private:
        LayerStack m_layerStack;
    };
}

extern std::unique_ptr<Engine::Application> createApplication();
