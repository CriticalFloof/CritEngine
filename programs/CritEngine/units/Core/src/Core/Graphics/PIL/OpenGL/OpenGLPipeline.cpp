#include "OpenGLPipeline.h"
#include "OpenGLShader.h"
#include "../../../Logging/Logger.h"

#include <glad/glad.h>
#include <memory>


namespace Engine
{
    OpenGLPipeline::OpenGLPipeline(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs)
    {
        std::shared_ptr<OpenGLShader> vertexShader = std::static_pointer_cast<OpenGLShader>(vs);
        std::shared_ptr<OpenGLShader> fragmentShader = std::static_pointer_cast<OpenGLShader>(fs);

        uint32_t program = glCreateProgram();
        this->m_programID = program;

        glAttachShader(program, vertexShader->shaderID);
        glAttachShader(program, fragmentShader->shaderID);

        glLinkProgram(program);

        int32_t is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
        if (is_linked == false)
        {
            int32_t max_message_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_message_length);

            std::vector<char> info_log(max_message_length);
            glGetProgramInfoLog(program, max_message_length, &max_message_length, &info_log[0]);

            glDeleteProgram(program);

            logError("OpenGL", "Shader Link Failure!");
            logError("OpenGL", std::string(info_log.begin(), info_log.end()));
            return;
        }

        glDetachShader(program, vertexShader->shaderID);
        glDetachShader(program, fragmentShader->shaderID);
    }

    OpenGLPipeline::~OpenGLPipeline()
    {
    }

    void OpenGLPipeline::bind() const
    {
        glUseProgram(this->m_programID);
    }

    void OpenGLPipeline::unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLPipeline::uploadUniformMat3(const std::string name, const Matrix3f& mat3)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat3.data[0]);
    }

    void OpenGLPipeline::uploadUniformMat4(const std::string name, const Matrix4f& mat4)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat4.data[0]);
    }

    void OpenGLPipeline::uploadUniformFloat(const std::string name, const float& float1)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniform1fv(location, 1, &float1);
    }

    void OpenGLPipeline::uploadUniformFloat2(const std::string name, const Vector2& float2)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniform2fv(location, 1, &float2.x);
    }

    void OpenGLPipeline::uploadUniformFloat3(const std::string name, const Vector3& float3)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniform3fv(location, 1, &float3.x);
    }

    void OpenGLPipeline::uploadUniformFloat4(const std::string name, const Vector4& float4)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniform4fv(location, 1, &float4.x);
    }

    void OpenGLPipeline::uploadUniformInt(const std::string name, const int& int1)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniform1iv(location, 1, &int1);
    }

    void OpenGLPipeline::uploadUniformBool(const std::string name, const int& boolean)
    {
        int location = glGetUniformLocation(this->m_programID, name.c_str());
        glUniform1iv(location, 1, &boolean);
    }
}
