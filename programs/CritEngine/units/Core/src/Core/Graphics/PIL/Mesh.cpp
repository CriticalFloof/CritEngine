#include "Mesh.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLMesh.h"

namespace Engine
{
    std::shared_ptr<Mesh> Mesh::create()
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported");
            return nullptr;

        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLMesh>();
        }

        ASSERT(false, "Invalid Render API!");
        return nullptr;
    }
}
