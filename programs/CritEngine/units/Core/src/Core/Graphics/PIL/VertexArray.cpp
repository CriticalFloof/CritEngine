#include "VertexArray.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLVertexArray.h"

namespace Engine {

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		ASSERT(false, "Invalid Render API!"); 
		return nullptr;
	}

}