#include <glad/glad.h>
#include <vector>
#include "OpenGLShader.h"
#include "../../../Logging/Logger.h"


namespace Engine
{
    uint32_t engineShaderTypeToOpenGLShaderType(ShaderType shader_type)
    {
        switch (shader_type)
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

    OpenGLShader::OpenGLShader(const std::string& glsl_source, const ShaderType shader_type)
        : shaderID(0)
    {
        ASSERT(shader_type != ShaderType::Task && shader_type != ShaderType::Mesh,
               "OpenGL doesn't support Task & Mesh Shaders!");

        this->shaderID = glCreateShader(engineShaderTypeToOpenGLShaderType(shader_type));
        const char* source = glsl_source.c_str();
        glShaderSource(this->shaderID, 1, &source, nullptr);
        glCompileShader(this->shaderID);

        int32_t is_compiled = 0;
        glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == false)
        {
            int32_t max_message_length = 0;
            glGetShaderiv(this->shaderID, GL_INFO_LOG_LENGTH, &max_message_length);

            std::vector<char> info_log(max_message_length);
            glGetShaderInfoLog(this->shaderID, max_message_length, &max_message_length, info_log.data());

            glDeleteShader(this->shaderID);

            logError("OpenGL", "Shader Compilation Failure!");
            logError("OpenGL", std::string(info_log.begin(), info_log.end()));

            return;
        }
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteShader(this->shaderID);
    }
}
