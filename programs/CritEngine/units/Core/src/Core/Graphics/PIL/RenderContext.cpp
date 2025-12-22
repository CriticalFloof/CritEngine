#include "RenderContext.h"
#include "../Renderer.h"
#include <GLFW/glfw3.h>

#include "OpenGL/OpenGLContext.h"

namespace Engine {

	std::shared_ptr<RenderContext> RenderContext::Create(GLFWwindow* windowHandle)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_unique<OpenGLContext>(windowHandle);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}