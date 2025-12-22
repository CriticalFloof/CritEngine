#include "OpenGLPipeline.h"
#include "OpenGLShader.h"
#include "../../../Logging/Logger.h"

#include <glad/glad.h>
#include <memory>


namespace Engine {

	OpenGLPipeline::OpenGLPipeline(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs)
	{
		std::shared_ptr<OpenGLShader> vertexShader = std::static_pointer_cast<OpenGLShader>(vs);
		std::shared_ptr<OpenGLShader> fragmentShader = std::static_pointer_cast<OpenGLShader>(fs);

		uint32_t program = glCreateProgram();
		this->programID = program;

		glAttachShader(program, vertexShader->shaderID);
		glAttachShader(program, fragmentShader->shaderID);

		glLinkProgram(program);

		int32_t isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == false)
		{
			int32_t maxMessageLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxMessageLength);

			std::vector<char> infoLog(maxMessageLength);
			glGetProgramInfoLog(program, maxMessageLength, &maxMessageLength, &infoLog[0]);

			glDeleteProgram(program);

			LogError("OpenGL", "Shader Link Failure!");
			LogError("OpenGL", std::string(infoLog.begin(), infoLog.end()));
			return;
		}

		glDetachShader(program, vertexShader->shaderID);
		glDetachShader(program, fragmentShader->shaderID);

	}
	OpenGLPipeline::~OpenGLPipeline()
	{

	}

	void OpenGLPipeline::Bind() const
	{
		glUseProgram(this->programID);
	}
	void OpenGLPipeline::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLPipeline::UploadUniformMat3(const std::string name, const Matrix3f& mat3)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &mat3.data[0]);
	}
	void OpenGLPipeline::UploadUniformMat4(const std::string name, const Matrix4f& mat4)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat4.data[0]);
	}
	void OpenGLPipeline::UploadUniformFloat(const std::string name, const float& float1)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniform1fv(location, 1, &float1);
	}
	void OpenGLPipeline::UploadUniformFloat2(const std::string name, const Vector2& float2)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniform2fv(location, 1, &float2.x);
	}
	void OpenGLPipeline::UploadUniformFloat3(const std::string name, const Vector3& float3)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniform3fv(location, 1, &float3.x);
	}
	void OpenGLPipeline::UploadUniformFloat4(const std::string name, const Vector4& float4)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniform4fv(location, 1, &float4.x);
	}
	void OpenGLPipeline::UploadUniformInt(const std::string name, const int& int1)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniform1iv(location, 1, &int1);
	}
	void OpenGLPipeline::UploadUniformBool(const std::string name, const int& boolean)
	{
		int location = glGetUniformLocation(this->programID, name.c_str());
		glUniform1iv(location, 1, &boolean);
	}
}