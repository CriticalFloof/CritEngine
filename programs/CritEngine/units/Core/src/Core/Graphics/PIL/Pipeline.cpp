#include "Pipeline.h"

#include "../Renderer.h"

#include "OpenGL/OpenGLPipeline.h"

namespace Engine {

	std::shared_ptr<Pipeline> Pipeline::Create(std::shared_ptr<Engine::Shader> vertexShader, std::shared_ptr<Engine::Shader> fragmentShader)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_unique<OpenGLPipeline>(vertexShader, fragmentShader);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}