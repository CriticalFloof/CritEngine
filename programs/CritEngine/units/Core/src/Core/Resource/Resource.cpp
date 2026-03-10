#include "Resource.h"
#include "../Core/GlobalEngine.h"

namespace Engine
{
    Resource::Resource()
        : id(0), dependencies(std::vector<UUID>()), sourcePath(""), resourceType("")
    {
    }

    Resource::Resource(std::string resource_type, std::filesystem::path filepath)
        : id(UUID()), dependencies(std::vector<UUID>()), sourcePath(filepath), resourceType(resource_type)
    {
    }

    std::shared_ptr<void> Resource::get() const
    {
        return GlobalEngine::get().getResourceManager().getResourceData(*this);
    };
}
