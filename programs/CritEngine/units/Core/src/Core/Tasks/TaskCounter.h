#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "Task.h"

#include "../Core/Base.h"

namespace Engine
{
    class TaskCounter
    {
    public:
        ENGINE_API TaskCounter();

        ENGINE_API void Add(uint32_t count);
        ENGINE_API void Signal();
        ENGINE_API void Wait();

    private:
        std::atomic<uint32_t> remaining;
        std::condition_variable condition;
        std::mutex mutex;
    };
}
