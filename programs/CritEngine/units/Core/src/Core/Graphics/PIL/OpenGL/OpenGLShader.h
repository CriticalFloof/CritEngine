#pragma once
#include "../Shader.h"

namespace Engine
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& glsl_source, ShaderType shader_type);
        ~OpenGLShader() override;

        uint32_t shaderID;
    };
}
