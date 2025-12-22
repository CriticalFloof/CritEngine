#pragma once
#include "../Pipeline.h"

namespace Engine {

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(std::shared_ptr<Engine::Shader> vertexShader, std::shared_ptr<Engine::Shader> fragmentShader);
		ENGINE_API virtual ~OpenGLPipeline() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformMat3(const std::string name, const Matrix3f& mat3) override;
		virtual void UploadUniformMat4(const std::string name, const Matrix4f& mat4) override;
		virtual void UploadUniformFloat(const std::string name, const float& float1) override;
		virtual void UploadUniformFloat2(const std::string name, const Vector2& float2) override;
		virtual void UploadUniformFloat3(const std::string name, const Vector3& float3) override;
		virtual void UploadUniformFloat4(const std::string name, const Vector4& float4) override;
		virtual void UploadUniformInt(const std::string name, const int& int1) override;
		virtual void UploadUniformBool(const std::string name, const int& boolean) override;
	private:
		uint32_t programID;
	};

}