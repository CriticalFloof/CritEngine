#pragma once

#include "../../Core/Base.h"
#include "Shader.h"

namespace Engine
{
    class Pipeline
    {
    public:
        ENGINE_API virtual ~Pipeline() = default;

        ENGINE_API static std::shared_ptr<Pipeline> create(std::shared_ptr<Shader> vertex_shader,
                                                           std::shared_ptr<Shader> fragment_shader);

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void uploadUniformMat3(std::string name, const Matrix3f& mat3) = 0;
        virtual void uploadUniformMat4(std::string name, const Matrix4f& mat4) = 0;
        virtual void uploadUniformFloat(std::string name, const float& float1) = 0;
        virtual void uploadUniformFloat2(std::string name, const Vector2& float2) = 0;
        virtual void uploadUniformFloat3(std::string name, const Vector3& float3) = 0;
        virtual void uploadUniformFloat4(std::string name, const Vector4& float4) = 0;
        virtual void uploadUniformInt(std::string name, const int& int1) = 0;
        virtual void uploadUniformBool(std::string name, const int& boolean) = 0;
    };
}
