#pragma once

#include "../ResourceLoader.h"
#include "../FileAccessor.h"
#include "../../Graphics/PIL/Texture.h"

namespace Engine
{
    //TODO: Add color palette support so that we can parse lower bpp formats.

    class BitmapLoader : public ResourceLoader
    {
    public:
        ~BitmapLoader() override
        {
        };

        std::string getResourceType() override;
        std::shared_ptr<void> load(std::filesystem::path filepath) override;
    };
}
