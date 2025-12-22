#pragma once

#include "PIL/RendererAPI.h"

namespace Engine {

	class RenderCommand
	{
	public:
		ENGINE_API inline static void SetClearColor(const glm::vec4& color)
		{
			RenderCommand::rendererAPI->SetClearColor(color);
		}

		ENGINE_API inline static void Clear()
		{
			RenderCommand::rendererAPI->Clear();
		}

		ENGINE_API inline static void EnableDepthTest(const  bool newState)
		{
			RenderCommand::rendererAPI->EnableDepthTest(newState);
		}

		ENGINE_API inline static void EnableDepthMask(const bool newState)
		{
			RenderCommand::rendererAPI->EnableDepthMask(newState);
		}

		ENGINE_API inline static void SetDepthTestFunc(const RendererAPI::DepthTestFunction newFunction)
		{
			RenderCommand::rendererAPI->SetDepthTestFunc(newFunction);
		}

		ENGINE_API inline static void SetViewportSize(const int width, const int height)
		{
			RenderCommand::rendererAPI->SetViewportSize(width, height);
		}

		ENGINE_API inline static void DrawIndexed(const std::shared_ptr<Mesh>& mesh)
		{
			RenderCommand::rendererAPI->DrawIndexed(mesh);
		}



	private:
		static std::unique_ptr<RendererAPI> rendererAPI;
	};

}