#include "Resource.h"
#include "../Core/GlobalEngine.h"

namespace Engine {
	Resource::Resource()
		: id(0), dependencies(std::vector<UUID>()), sourcePath(""), resourceType("")
	{

	}

	Resource::Resource(std::string resourceType, std::filesystem::path filepath)
		: id(UUID()), dependencies(std::vector<UUID>()), sourcePath(filepath), resourceType(resourceType)
	{

	}

	Resource::~Resource() {}

	std::shared_ptr<void> Resource::Get() const
	{
		return GlobalEngine::Get().GetResourceManager().GetResourceData(*this);
	};
}