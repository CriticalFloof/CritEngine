#include "Shader.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLShader.h"

namespace Engine {

	std::shared_ptr<Shader> Shader::Create(const std::string& glslSource, const ShaderType shaderType)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(glslSource, shaderType);
		}

		ASSERT(false, "Invalid Render API!");
		return nullptr;
	}

}