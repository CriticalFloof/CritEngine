#pragma once

#include "../Texture.h"

namespace Engine {
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(char* vertices, uint32_t width, uint32_t height, uint8_t channelCount);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() override { return this->width; }
		virtual uint32_t GetHeight() override { return this->height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t textureID;
		uint32_t width;
		uint32_t height;
	};
}