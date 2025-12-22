#pragma once

#include "../ResourceLoader.h"
#include "../FileAccessor.h"
#include "../../Graphics/PIL/Texture.h"

namespace Engine {

	//TODO: Add color palette support so that we can parse lower bpp formats.

	class BitmapLoader : public ResourceLoader
	{
	public:
		virtual ~BitmapLoader() override {};

		virtual std::string GetResourceType() override;
		virtual std::shared_ptr<void> Load(std::filesystem::path filepath) override;		
		
	};

}