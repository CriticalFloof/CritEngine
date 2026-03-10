#include "Shader.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLShader.h"

namespace Engine
{
    std::shared_ptr<Shader> Shader::create(const std::string& glsl_source, const ShaderType shader_type)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported");
            return nullptr;

        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(glsl_source, shader_type);
        }

        ASSERT(false, "Invalid Render API!");
        return nullptr;
    }
}
