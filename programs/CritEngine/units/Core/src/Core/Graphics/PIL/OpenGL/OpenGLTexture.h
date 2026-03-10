#pragma once

#include "../Texture.h"

namespace Engine
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(char* vertices, uint32_t width, uint32_t height, uint8_t channel_count);
        ~OpenGLTexture() override;

        uint32_t getWidth() override { return this->m_width; }
        uint32_t getHeight() override { return this->m_height; }

        void bind(uint32_t slot = 0) const override;

    private:
        uint32_t m_textureID;
        uint32_t m_width;
        uint32_t m_height;
    };
}
