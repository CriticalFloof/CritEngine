#include "Texture.h"
#include "../Renderer.h"

#include "OpenGL/OpenGLTexture.h"

namespace Engine {

	std::shared_ptr<Texture> Texture::Create(char* data, uint32_t width, uint32_t height, uint8_t channelCount)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture>(data, width, height, channelCount);
		}

		ASSERT(false, "Invalid Render API!");
		return nullptr;
	}

	std::unique_ptr<Texture> Texture::CreateUnique(char* data, uint32_t width, uint32_t height, uint8_t channelCount)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false, "None RenderAPI is currently not supported"); return nullptr;

		case RendererAPI::API::OpenGL: return std::make_unique<OpenGLTexture>(data, width, height, channelCount);
		}

		ASSERT(false, "Invalid Render API!");
		return nullptr;
	}

}