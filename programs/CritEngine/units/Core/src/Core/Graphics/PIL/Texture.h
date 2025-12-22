#pragma once

#include "../../Core/Base.h"

namespace Engine {

	class Texture
	{
	public:
		virtual ~Texture() {};

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		ENGINE_API static std::shared_ptr<Texture> Create(char* data, uint32_t width, uint32_t height, uint8_t channelCount);
		ENGINE_API static std::unique_ptr<Texture> CreateUnique(char* data, uint32_t width, uint32_t height, uint8_t channelCount);
	};

}