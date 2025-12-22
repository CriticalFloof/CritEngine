#pragma once
#include <cstdint>

namespace ECS {
	using EntityIndex = uint32_t;
	using EntityVersion = uint32_t;
	using EntityID = uint64_t;

	namespace EntityUtils {
		inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
		{
			return ((EntityID)index << 32) | ((EntityID)version);
		}
		inline EntityIndex GetEntityIndex(EntityID id)
		{
			return id >> 32;
		}
		inline EntityVersion GetEntityVersion(EntityID id)
		{
			return (EntityVersion)id;
		}
		inline bool IsEntityValid(EntityID id)
		{
			return (id >> 32) != EntityIndex(-1);
		}
	}
}