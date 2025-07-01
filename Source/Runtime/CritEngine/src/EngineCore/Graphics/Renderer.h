#pragma once

#include "Camera.h"
#include "RenderCommand.h"

#include "Model.h"

namespace Engine {

	class Renderer
	{
	public:

		ENGINE_API static void BeginScene(std::shared_ptr<PerspectiveCamera> camera);
		ENGINE_API static void EndScene();

		ENGINE_API static void Submit(const std::shared_ptr<Model>& model);

		ENGINE_API inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:

		static std::shared_ptr<PerspectiveCamera> activeCamera;

	};
}
