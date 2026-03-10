#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"

namespace Engine
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };

        enum class DepthTestFunction : uint8_t
        {
            Always,
            Never,
            Equal,
            NotEqual,
            Less,
            LessOrEqual,
            Greater,
            GreaterOrEqual
        };

        ENGINE_API virtual ~RendererAPI() = default;

        ENGINE_API static std::unique_ptr<RendererAPI> create();

        static API getAPI() { return m_api; }

        virtual void setClearColor(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual void enableDepthTest(bool new_state) = 0;
        virtual void enableDepthMask(bool new_state) = 0;
        virtual void setDepthTestFunc(DepthTestFunction new_function) = 0;

        virtual void setViewportSize(int width, int height) = 0;

        virtual void drawIndexed(const std::shared_ptr<Mesh>& vertex_array) = 0;

    private:
        static API m_api;
    };
}
