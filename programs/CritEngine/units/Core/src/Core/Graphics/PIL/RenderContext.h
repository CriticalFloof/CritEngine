#pragma once

#include "../../Core/Base.h"

struct GLFWwindow;

namespace Engine
{
    class RenderContext
    {
    public:
        ENGINE_API virtual ~RenderContext() = default;

        ENGINE_API static std::shared_ptr<RenderContext> create(GLFWwindow* window_handle);

        virtual void init() = 0;
        virtual void swapBuffers() = 0;
        virtual void initImGui() = 0;
        virtual void imGuiStartFrame() = 0;
        virtual void imGuiRender() = 0;

    protected:
        GLFWwindow* m_windowHandle;
    };
}
