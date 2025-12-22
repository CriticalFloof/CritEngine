#pragma once

#include "../RendererAPI.h"

namespace Engine {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual ~OpenGLRendererAPI() override {};

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void EnableDepthTest(const bool newState) override;
		virtual void EnableDepthMask(const bool newState) override;
		virtual void SetDepthTestFunc(const DepthTestFunction newFunction) override;

		virtual void SetViewportSize(const int width, const int height) override;

		virtual void DrawIndexed(const std::shared_ptr<Mesh>& mesh) override;
	};

}