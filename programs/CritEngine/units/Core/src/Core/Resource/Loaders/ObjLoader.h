#pragma once
#include "../ResourceLoader.h"
#include "../FileAccessor.h"
#include "../../Graphics/PIL/Mesh.h"

namespace Engine
{
    class ObjLoader : public ResourceLoader
    {
    public:
        ~ObjLoader() override
        {
        }

        std::string getResourceType() override;
        std::shared_ptr<void> load(std::filesystem::path filepath) override;
    };
}
