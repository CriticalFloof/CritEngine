#include "TaskCounter.h"

namespace Engine
{
    // This helper class tracks the number of pending tasks and is used internally in the TaskScheduler. 
    // The Wait() function is wrapped in the TaskScheduler and is used to synchronize with the completion of a..
    // batch of tasks.

    TaskCounter::TaskCounter()
        : m_remaining(0)
    {
    }

    void TaskCounter::add(uint32_t count)
    {
        ASSERT(count > 0, "Can't add zero tasks");
        this->m_remaining.fetch_add(count, std::memory_order_relaxed);
    }

    void TaskCounter::signal()
    {
        if (this->m_remaining.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            this->m_condition.notify_all();
        }
    }

    void TaskCounter::wait()
    {
        std::unique_lock<std::mutex> lock(this->m_mutex);
        this->m_condition.wait(lock, [this]()
        {
            return this->m_remaining.load(std::memory_order_acquire) == 0;
        });
    }
}
