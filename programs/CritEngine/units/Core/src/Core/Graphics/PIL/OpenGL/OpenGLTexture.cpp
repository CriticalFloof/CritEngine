#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stdio.h>
#include <gl/GLU.h>

namespace Engine
{
    OpenGLTexture::OpenGLTexture(char* data, uint32_t width, uint32_t height, uint8_t channel_count)
        : m_width(width), m_height(height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &this->m_textureID);
        glTextureStorage2D(this->m_textureID, 1, GL_RGBA8, width, height);

        glTextureParameteri(this->m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(this->m_textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(this->m_textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(this->m_textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &this->m_textureID);
    }

    void OpenGLTexture::bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, this->m_textureID);
    }
}
