#pragma once

#include <string>
#include <filesystem>

#include "Resource.h"

namespace Engine {

	class ResourceLoader
	{
	public:
		virtual ~ResourceLoader() {};

		virtual std::string GetResourceType() = 0;
		virtual std::shared_ptr<void> Load(std::filesystem::path filepath) = 0;
	};

}