#include "GraphicsEntry.h"
#include "../Renderer.h"

namespace Engine {

	std::unique_ptr<GraphicsEntry> GraphicsEntry::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::unique_ptr<GraphicsEntry>();
		}

		ASSERT(false, "Invalid Render API!");
		return nullptr;
	}
}