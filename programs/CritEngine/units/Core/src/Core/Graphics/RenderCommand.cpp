#include "RenderCommand.h"

namespace Engine
{
    std::unique_ptr<RendererAPI> RenderCommand::m_rendererAPI = RendererAPI::create();
}
