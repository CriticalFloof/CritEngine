#include "ResourceManager.h"

namespace Engine {

	ResourceManager::ResourceManager()
	{
	}

	void ResourceManager::RegisterLoader(std::unique_ptr<ResourceLoader> loader)
	{
		this->registeredLoaders[loader->GetResourceType()] = std::move(loader);
		std::weak_ptr<void> e = std::shared_ptr<void>(nullptr);
	}

	std::shared_ptr<void> ResourceManager::GetResourceData(const Resource& resourceHandle)
	{
		if (auto cacheEntry = this->resourceDataCache.find(resourceHandle.id); cacheEntry != this->resourceDataCache.end())
		{
			std::shared_ptr<void> asset = cacheEntry->second.lock();
			return asset;
		}

		return this->registeredLoaders[resourceHandle.resourceType]->Load(resourceHandle.sourcePath);
	}
}