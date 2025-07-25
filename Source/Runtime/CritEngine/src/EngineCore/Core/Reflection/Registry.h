#pragma once
#include <unordered_map>
#include <typeindex>
#include <cstdio>

#include "../Base.h"
#include "TypeInfo.h"

namespace Reflection {

	/*
	DLL-safe handle into the reflection system's type registry.
	*/
	class ENGINE_API TypeRegistryHandle
	{
	public:
		void SetMetadata(std::type_index type, std::unique_ptr<TypeInfo> metadata);
		const TypeInfo* GetMetadata(std::type_index type);
	private:
		class TypeRegistry;
		TypeRegistry* impl;
	};

	/*
	Allows users to leverage static initialization to automatically register type metadata.
	*/
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