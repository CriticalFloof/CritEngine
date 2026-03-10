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

        ENGINE_API void add(uint32_t count);
        ENGINE_API void signal();
        ENGINE_API void wait();

    private:
        std::atomic<uint32_t> m_remaining;
        std::condition_variable m_condition;
        std::mutex m_mutex;
    };
}
