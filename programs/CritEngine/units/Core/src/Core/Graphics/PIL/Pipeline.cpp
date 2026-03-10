#include "Pipeline.h"

#include "../Renderer.h"

#include "OpenGL/OpenGLPipeline.h"

namespace Engine
{
    std::shared_ptr<Pipeline> Pipeline::create(std::shared_ptr<Shader> vertex_shader,
                                               std::shared_ptr<Shader> fragment_shader)
    {
        switch (RendererAPI::getAPI())
        {
        case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_unique<OpenGLPipeline>(vertex_shader, fragment_shader);
        }

        ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
