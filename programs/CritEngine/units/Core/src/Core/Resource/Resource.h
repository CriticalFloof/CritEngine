#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include "../Core/Base.h"
#include "../Core/UUID.h"


namespace Engine
{
    class Resource
    {
    public:
        ENGINE_API Resource();
        ENGINE_API Resource(std::string resource_type, std::filesystem::path filepath);

        ENGINE_API std::shared_ptr<void> get() const;

        UUID id;
        std::vector<UUID> dependencies;
        std::filesystem::path sourcePath;
        std::string resourceType;
    };
}
