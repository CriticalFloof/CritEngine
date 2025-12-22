#include "Mesh.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLMesh.h"

namespace Engine {

	std::shared_ptr<Mesh> Mesh::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLMesh>();
		}

		ASSERT(false, "Invalid Render API!");
		return nullptr;
	}

}