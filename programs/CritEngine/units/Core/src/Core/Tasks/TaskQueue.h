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

        ENGINE_API void PushBack(const Task& task);
        ENGINE_API void PushFront(const Task& task);

        ENGINE_API std::optional<Task> PopBack();
        ENGINE_API std::optional<Task> StealFront();

        ENGINE_API bool IsEmpty();

    private:
        std::deque<Task> deque;
        std::mutex mutex;
    };
}