#pragma once

#include "../RenderContext.h"
#include "../../../Window/Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext() override;

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void InitImGui() override;
		virtual void ImGuiStartFrame() override;
		virtual void ImGuiRender() override;
	};
}