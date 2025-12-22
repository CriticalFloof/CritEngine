#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"

namespace Engine {

	RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_unique<OpenGLRendererAPI>();
		}

		ASSERT(false, "Unknown RendererAPI!"); 
		return nullptr;
	}
}