#pragma once
#include "../ResourceLoader.h"
#include "../FileAccessor.h"
#include "../../Graphics/PIL/Mesh.h"

namespace Engine {

	class ObjLoader : public ResourceLoader
	{
	public:
		virtual ~ObjLoader() override {};

		virtual std::string GetResourceType() override;
		virtual std::shared_ptr<void> Load(std::filesystem::path filepath) override;
	};

}