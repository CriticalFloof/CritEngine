#pragma once
#include <unordered_map>
#include <typeindex>
#include <cstdio>

#include "../Base.h"
#include "KindInfoInterface.h"

namespace Reflection {

	// Pointer to a registry owned, const KindInfo.
	using KindInfoRegistryEntry = const KindInfo*;

	/*
	DLL-safe handle into the reflection system's type registry.
	*/
	class ENGINE_API KindRegistryHandle
	{
	public:
		void SetMetadata(std::type_index type, std::unique_ptr<KindInfo> metadata);
		KindInfoRegistryEntry GetMetadata(std::type_index type);
	private:
		class KindRegistry;
		KindRegistry* impl;
	};

	/*
	Allows users to leverage static initialization to automatically register type metadata.
	*/
	class ENGINE_API KindRegistrar
	{
	public:
		KindRegistrar(std::type_index type, std::unique_ptr<KindInfo> metadata)
		{
			this->registryHandle.SetMetadata(type, std::move(metadata));
		}
	private:
		static KindRegistryHandle registryHandle;
	};

}