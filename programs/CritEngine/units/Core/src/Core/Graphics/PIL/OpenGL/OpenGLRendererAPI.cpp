#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Engine
{
    static GLenum depthFuncTypeToOpenGlDepthFuncType(RendererAPI::DepthTestFunction type)
    {
        switch (type)
        {
        case RendererAPI::DepthTestFunction::Always: return GL_ALWAYS;
        case RendererAPI::DepthTestFunction::Never: return GL_NEVER;
        case RendererAPI::DepthTestFunction::Greater: return GL_GREATER;
        case RendererAPI::DepthTestFunction::GreaterOrEqual: return GL_GEQUAL;
        case RendererAPI::DepthTestFunction::Less: return GL_LESS;
        case RendererAPI::DepthTestFunction::LessOrEqual: return GL_LEQUAL;
        case RendererAPI::DepthTestFunction::Equal: return GL_EQUAL;
        case RendererAPI::DepthTestFunction::NotEqual: return GL_NOTEQUAL;
        }

        ASSERT(false, "Unknown ShaderDataType.")
        return 0;
    }

    void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::enableDepthTest(const bool new_state)
    {
        new_state ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::enableDepthMask(const bool new_state)
    {
        glDepthMask(new_state);
    }

    void OpenGLRendererAPI::setDepthTestFunc(const DepthTestFunction new_function)
    {
        glDepthFunc(depthFuncTypeToOpenGlDepthFuncType(new_function));
    }

    void OpenGLRendererAPI::setViewportSize(const int width, const int height)
    {
        glViewport(0, 0, width, height);
    }

    void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<Mesh>& mesh)
    {
        glDrawElements(GL_TRIANGLES, mesh->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }
}
