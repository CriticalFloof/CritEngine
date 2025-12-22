#include "RenderCommand.h"

namespace Engine {

	std::unique_ptr<RendererAPI> RenderCommand::rendererAPI = RendererAPI::Create();

}