#pragma once
#include <deque>
#include <mutex>
#include <optional>

#include "Task.h"
#include "../Core/Base.h"

namespace Engine
{
    class TaskQueue
    {
    public:
        ENGINE_API TaskQueue() = default;
        ENGINE_API ~TaskQueue() = default;

        // Explicitly delete copy and move operations
        TaskQueue(const TaskQueue&) = delete;
        TaskQueue& operator=(const TaskQueue&) = delete;
        TaskQueue(TaskQueue&&) = delete;
        TaskQueue& operator=(TaskQueue&&) = delete;

        ENGINE_API void pushBack(const Task& task);
        ENGINE_API void pushFront(const Task& task);

        ENGINE_API std::optional<Task> popBack();
        ENGINE_API std::optional<Task> stealFront();

        ENGINE_API bool isEmpty();

    private:
        std::deque<Task> m_deque;
        std::mutex m_mutex;
    };
}
