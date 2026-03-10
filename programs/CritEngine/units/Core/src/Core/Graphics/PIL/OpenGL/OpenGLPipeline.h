#pragma once
#include "../Pipeline.h"

namespace Engine
{
    class OpenGLPipeline : public Pipeline
    {
    public:
        OpenGLPipeline(std::shared_ptr<Shader> vertex_shader, std::shared_ptr<Shader> fragment_shader);
        ENGINE_API ~OpenGLPipeline() override;

        void bind() const override;
        void unbind() const override;

        void uploadUniformMat3(std::string name, const Matrix3f& mat3) override;
        void uploadUniformMat4(std::string name, const Matrix4f& mat4) override;
        void uploadUniformFloat(std::string name, const float& float1) override;
        void uploadUniformFloat2(std::string name, const Vector2& float2) override;
        void uploadUniformFloat3(std::string name, const Vector3& float3) override;
        void uploadUniformFloat4(std::string name, const Vector4& float4) override;
        void uploadUniformInt(std::string name, const int& int1) override;
        void uploadUniformBool(std::string name, const int& boolean) override;

    private:
        uint32_t m_programID;
    };
}
