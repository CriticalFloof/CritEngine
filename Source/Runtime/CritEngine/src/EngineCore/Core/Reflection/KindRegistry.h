#pragma once
#include <unordered_map>
#include <typeindex>
#include <cstdio>

#include "../Base.h"
#include "KindInfoInterface.h"

#define REFLECT_PRIMITIVE(name, type) KindRegistrar registerKind_##name = KindRegistrar(typeid(type), std::make_unique<KindInfo>(KindInfo::Create<##type>(#type)));

namespace Reflection {

	/*
	DLL-safe handle into the reflection system's type registry.
	*/
	class ENGINE_API KindRegistryHandle
	{
	public:
		void SetMetadata(std::type_index type, std::unique_ptr<KindInfo> metadata);
		const KindInfo* GetMetadata(std::type_index type);
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