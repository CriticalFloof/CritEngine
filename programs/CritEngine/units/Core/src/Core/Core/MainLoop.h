#pragma once

#include <memory>

#include "Base.h"
#include "../Logging/Logger.h"


namespace Engine
{
    class MainLoop
    {
    public:
        ENGINE_API MainLoop();
        ENGINE_API void run();
        ENGINE_API void tick();
        ENGINE_API void stop();

    private:
        bool m_is_running;
    };
}
