#pragma once

#include <string>
#include <filesystem>

#include "Resource.h"

namespace Engine
{
    class ResourceLoader
    {
    public:
        ResourceLoader() = default;
        ResourceLoader(const ResourceLoader&) = default;
        ResourceLoader& operator=(const ResourceLoader&) = default;
        virtual ~ResourceLoader() = default;
        
        virtual std::string getResourceType() = 0;
        virtual std::shared_ptr<void> load(std::filesystem::path filepath) = 0;
    };
}
