#include "ResourceManager.h"

namespace Engine
{
    ResourceManager::ResourceManager()
    {
    }

    void ResourceManager::registerLoader(std::unique_ptr<ResourceLoader> loader)
    {
        this->m_registeredLoaders[loader->getResourceType()] = std::move(loader);
        std::weak_ptr<void> e = std::shared_ptr<void>(nullptr);
    }

    std::shared_ptr<void> ResourceManager::getResourceData(const Resource& resource_handle)
    {
        if (auto cacheEntry = this->m_resourceDataCache.find(resource_handle.id); cacheEntry != this->m_resourceDataCache.
            end())
        {
            std::shared_ptr<void> asset = cacheEntry->second.lock();
            return asset;
        }

        return this->m_registeredLoaders[resource_handle.resourceType]->load(resource_handle.sourcePath);
    }
}
