#pragma once

#include "../RenderContext.h"
#include "../../../Window/Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace Engine
{
    class OpenGLContext : public RenderContext
    {
    public:
        OpenGLContext(GLFWwindow* window_handle);
        ~OpenGLContext() override;

        void init() override;
        void swapBuffers() override;
        void initImGui() override;
        void imGuiStartFrame() override;
        void imGuiRender() override;
    };
}
