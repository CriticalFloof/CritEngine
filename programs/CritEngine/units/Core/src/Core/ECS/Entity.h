#pragma once
#include <cstdint>

namespace ECS
{
    using EntityIndex = uint32_t;
    using EntityVersion = uint32_t;
    using EntityID = uint64_t;

    namespace EntityUtils
    {
        inline EntityID createEntityId(EntityIndex index, EntityVersion version)
        {
            return (static_cast<EntityID>(index) << 32) | static_cast<EntityID>(version);
        }

        inline EntityIndex getEntityIndex(EntityID id)
        {
            return id >> 32;
        }

        inline EntityVersion getEntityVersion(EntityID id)
        {
            return static_cast<EntityVersion>(id);
        }

        inline bool isEntityValid(EntityID id)
        {
            return (id >> 32) != static_cast<EntityIndex>(-1);
        }
    }
}
