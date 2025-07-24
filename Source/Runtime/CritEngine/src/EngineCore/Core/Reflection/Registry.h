#pragma once
#include <unordered_map>
#include <typeindex>
#include <cstdio>

#include "../Base.h"
#include "TypeInfo.h"

namespace Reflection {

	class ENGINE_API TypeRegistryHandle
	{
	public:
		void SetMetadata(std::type_index type, std::unique_ptr<TypeInfo> metadata);
		const TypeInfo* GetMetadata(std::type_index type);
	private:
		class TypeRegistry;
		TypeRegistry* impl;
	};

	class ENGINE_API TypeRegistrar
	{
	public:
		TypeRegistrar(std::type_index type, std::unique_ptr<TypeInfo> metadata)
		{
			this->registryHandle.SetMetadata(type, std::move(metadata));
		}
	private:
		static TypeRegistryHandle registryHandle;
	};

}