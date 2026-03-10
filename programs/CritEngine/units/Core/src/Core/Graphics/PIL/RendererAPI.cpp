#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
    RendererAPI::API RendererAPI::m_api = API::OpenGL;

    std::unique_ptr<RendererAPI> RendererAPI::create()
    {
        switch (getAPI())
        {
        case API::None: ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case API::OpenGL: return std::make_unique<OpenGLRendererAPI>();
        }

        ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }
}
