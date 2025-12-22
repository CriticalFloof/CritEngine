#pragma once
#include "../Shader.h"

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& glslSource, const ShaderType shaderType);
		virtual ~OpenGLShader();

		uint32_t shaderID;
	};

}