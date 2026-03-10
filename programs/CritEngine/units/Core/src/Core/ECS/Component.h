#pragma once
#include <stdlib.h>
#include "../Core/Base.h"

namespace ECS
{
    struct Component
    {
    };

    static int g_component_id_counter = 0;

    template <class T>
    int getId()
    {
        static int component_id = g_component_id_counter++;
        return component_id;
    };

    // Barebones runtime array memory allocator, type safety must be handled by its user.
    struct ComponentMemoryPool
    {
        ComponentMemoryPool(size_t elementSize, size_t maxEntities, size_t elementAlignment)
        {
            constexpr int buffer_alignment = 64;
            ASSERT(buffer_alignment >= elementAlignment, "Components must have an alignment of 64 bytes or less.");

            this->stride = ((elementSize + elementAlignment - 1) / elementAlignment) * elementAlignment;

            // This has potential misalignment issues if the component has an alignment larger, on x86 architectures this will slowdown execution dramatically, while arm architectures will crash.
            this->data = new char[this->stride * maxEntities];
        }

        ~ComponentMemoryPool()
        {
            delete[] this->data;
        }

        void* get(size_t index)
        {
            return this->data + index * this->stride;
        }

        char* data{nullptr};
        size_t stride{0};
    };
}
