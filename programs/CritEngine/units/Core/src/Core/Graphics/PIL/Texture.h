#pragma once

#include "../../Core/Base.h"

namespace Engine
{
    class Texture
    {
    public:
        virtual ~Texture()
        {
        };

        virtual uint32_t getWidth() = 0;
        virtual uint32_t getHeight() = 0;

        virtual void bind(uint32_t slot = 0) const = 0;

        ENGINE_API static std::shared_ptr<Texture> create(char* data, uint32_t width, uint32_t height,
                                                          uint8_t channel_count);
        ENGINE_API static std::unique_ptr<Texture> createUnique(char* data, uint32_t width, uint32_t height,
                                                                uint8_t channel_count);
    };
}
