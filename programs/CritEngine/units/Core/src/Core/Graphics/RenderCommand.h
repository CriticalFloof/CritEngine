#pragma once

#include "PIL/RendererAPI.h"

namespace Engine
{
    class RenderCommand
    {
    public:
        ENGINE_API static void setClearColor(const glm::vec4& color)
        {
            m_rendererAPI->setClearColor(color);
        }

        ENGINE_API static void clear()
        {
            m_rendererAPI->clear();
        }

        ENGINE_API static void enableDepthTest(const bool new_state)
        {
            m_rendererAPI->enableDepthTest(new_state);
        }

        ENGINE_API static void enableDepthMask(const bool new_state)
        {
            m_rendererAPI->enableDepthMask(new_state);
        }

        ENGINE_API static void setDepthTestFunc(const RendererAPI::DepthTestFunction new_function)
        {
            m_rendererAPI->setDepthTestFunc(new_function);
        }

        ENGINE_API static void setViewportSize(const int width, const int height)
        {
            m_rendererAPI->setViewportSize(width, height);
        }

        ENGINE_API static void drawIndexed(const std::shared_ptr<Mesh>& mesh)
        {
            m_rendererAPI->drawIndexed(mesh);
        }

    private:
        static std::unique_ptr<RendererAPI> m_rendererAPI;
    };
}
