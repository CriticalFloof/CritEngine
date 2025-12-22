#pragma once
#include <stdlib.h>
#include "../Core/Base.h"

namespace ECS {
    struct Component {};

    static int componentIdCounter = 0;
    template <class T>
    int GetId()
    {
        static int componentId = componentIdCounter++;
        return componentId;
    };

    // Barebones runtime array memory allocator, type safety must be handled by its user.
    struct ComponentMemoryPool {
        ComponentMemoryPool(size_t elementSize, size_t maxEntities, size_t elementAlignment)
        {
            const int BUFFER_ALIGNMENT = 64;
            ASSERT(BUFFER_ALIGNMENT >= elementAlignment, "Components must have an alignment of 64 bytes or less.");

            this->stride = ((elementSize + elementAlignment - 1) / elementAlignment) * elementAlignment;

            // This has potential misalignment issues if the component has an alignment larger, on x86 architectures this will slowdown execution dramatically, while arm architectures will crash.
            this->data = new char[this->stride * maxEntities];
        }

        ~ComponentMemoryPool()
        {
            delete[] this->data;
        }

        inline void* Get(size_t index)
        {
            return this->data + index * this->stride;
        }

        char* data { nullptr };
        size_t stride { 0 };
    };
}

