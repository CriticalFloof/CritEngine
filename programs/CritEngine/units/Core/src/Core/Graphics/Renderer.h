#pragma once

#include "Camera.h"
#include "RenderCommand.h"

#include "Model.h"

namespace Engine
{
    class Renderer
    {
    public:
        ENGINE_API static void beginScene(std::shared_ptr<BaseCamera> camera);
        ENGINE_API static void endScene();

        ENGINE_API static void submit(const std::shared_ptr<Model>& model);

        ENGINE_API static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        static std::shared_ptr<BaseCamera> m_activeCamera;
    };
}
