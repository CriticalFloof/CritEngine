#include "RenderContext.h"
#include "../Renderer.h"
#include <GLFW/glfw3.h>

#include "OpenGL/OpenGLContext.h"

namespace Engine
{
    std::shared_ptr<RenderContext> RenderContext::create(GLFWwindow* window_handle)
    {
        switch (RendererAPI::getAPI())
        {
        case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_unique<OpenGLContext>(window_handle);
        }

        ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
