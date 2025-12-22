#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

#include "../Core/Base.h"
#include "Resource.h"
#include "ResourceLoader.h"

namespace Engine {

	class ResourceManager
	{
	public:
		ResourceManager();

		ENGINE_API void RegisterLoader(std::unique_ptr<ResourceLoader> loader);
		ENGINE_API std::shared_ptr<void> GetResourceData(const Resource& resourceHandle);

	private:
		std::unordered_map<std::string, std::unique_ptr<ResourceLoader>> registeredLoaders;

		std::unordered_map<UUID, std::weak_ptr<void>> resourceDataCache;
	};

}