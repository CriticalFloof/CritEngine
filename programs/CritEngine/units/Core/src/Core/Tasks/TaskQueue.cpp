#include "TaskQueue.h"

namespace Engine
{
    // Double-ended queue designed for use with the work-stealing pattern. Each worker thread owns a queue.

    void TaskQueue::pushBack(const Task& task)
    {
        std::lock_guard<std::mutex> lock(this->m_mutex);
        this->m_deque.emplace_back(task);
    }

    void TaskQueue::pushFront(const Task& task)
    {
        std::lock_guard<std::mutex> lock(this->m_mutex);
        this->m_deque.emplace_front(task);
    }

    // Returns std::nullopt if the queue is empty. 
    std::optional<Task> TaskQueue::popBack()
    {
        std::lock_guard<std::mutex> lock(this->m_mutex);
        if (this->m_deque.empty()) return std::nullopt;
        Task task = m_deque.back();
        this->m_deque.pop_back();
        return task;
    }

    std::optional<Task> TaskQueue::stealFront()
    {
        std::lock_guard<std::mutex> lock(this->m_mutex);
        if (this->m_deque.empty()) return std::nullopt;
        Task task = this->m_deque.front();
        this->m_deque.pop_front();
        return task;
    }

    bool TaskQueue::isEmpty()
    {
        std::lock_guard<std::mutex> lock(this->m_mutex);
        return this->m_deque.empty();
    }
}
