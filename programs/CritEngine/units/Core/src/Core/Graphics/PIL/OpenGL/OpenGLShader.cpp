#include <glad/glad.h>
#include <vector>
#include "OpenGLShader.h"
#include "../../../Logging/Logger.h"


namespace Engine {

	uint32_t EngineShaderTypeToOpenGLShaderType(ShaderType shaderType)
	{
		switch (shaderType)
		{
			case ShaderType::Vertex: return GL_VERTEX_SHADER;
			case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
			case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
			case ShaderType::TessellationControl: return GL_TESS_CONTROL_SHADER;
			case ShaderType::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
			case ShaderType::Compute: return GL_COMPUTE_SHADER;
			default: return GL_NONE;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& glslSource, const ShaderType shaderType)
		: shaderID(0)
	{
		ASSERT(shaderType != ShaderType::Task && shaderType != ShaderType::Mesh, "OpenGL doesn't support Task & Mesh Shaders!");

		this->shaderID = glCreateShader(EngineShaderTypeToOpenGLShaderType(shaderType));
		const char* source = glslSource.c_str();
		glShaderSource(this->shaderID, 1, &source, 0);
		glCompileShader(this->shaderID);

		int32_t isCompiled = 0;
		glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false)
		{
			int32_t maxMessageLength = 0;
			glGetShaderiv(this->shaderID, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetShaderInfoLog(this->shaderID, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteShader(this->shaderID);

			LogError("OpenGL", "Shader Compilation Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));

			return;
		}
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteShader(this->shaderID);
	}
}