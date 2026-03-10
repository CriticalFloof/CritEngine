#pragma once

#include "../RendererAPI.h"

namespace Engine
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        ~OpenGLRendererAPI() override
        {
        };

        void setClearColor(const glm::vec4& color) override;
        void clear() override;

        void enableDepthTest(bool new_state) override;
        void enableDepthMask(bool new_state) override;
        void setDepthTestFunc(DepthTestFunction new_function) override;

        void setViewportSize(int width, int height) override;

        void drawIndexed(const std::shared_ptr<Mesh>& mesh) override;
    };
}
