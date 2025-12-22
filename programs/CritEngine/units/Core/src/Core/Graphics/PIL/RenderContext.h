#pragma once

#include "../../Core/Base.h"

struct GLFWwindow;

namespace Engine {

	class RenderContext
	{
	public:
		ENGINE_API virtual ~RenderContext() = default;

		ENGINE_API static std::shared_ptr<RenderContext> Create(GLFWwindow* windowHandle);

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void InitImGui() = 0;
		virtual void ImGuiStartFrame() = 0;
		virtual void ImGuiRender() = 0;
	protected:
		GLFWwindow* windowHandle;
	};
}