#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stdio.h>
#include <gl/GLU.h>

namespace Engine {

	OpenGLTexture::OpenGLTexture(char* data, uint32_t width, uint32_t height, uint8_t channelCount)
		: width(width), height(height)
	{

		glCreateTextures(GL_TEXTURE_2D, 1, &this->textureID);
		glTextureStorage2D(this->textureID, 1, GL_RGBA8, width, height);

		glTextureParameteri(this->textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(this->textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(this->textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(this->textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &this->textureID);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, this->textureID);
	}
}