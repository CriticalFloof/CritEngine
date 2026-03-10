#pragma once
#include <string>
#include "../../Core/Base.h"
#include "../../Math/Common.h"


namespace Engine
{
    enum class ShaderType : uint8_t
    {
        Vertex,
        Fragment,
        Geometry,
        Mesh,
        Task,
        TessellationControl,
        TessellationEvaluation,
        Compute
    };

    class Shader
    {
    public:
        ENGINE_API static std::shared_ptr<Shader> create(const std::string& glsl_source, ShaderType shader_type);

        virtual ~Shader()
        {
        };
    };
}
