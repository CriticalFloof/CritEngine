#pragma once
#include <thread>
#include <vector>
#include <atomic>
#include <random>
#include <memory>
#include <condition_variable>

#include "../Threading/ThreadingHelpers.h"
#include "TaskQueue.h"
#include "TaskCounter.h"
#include "../Core/Base.h"

namespace Engine
{
    class TaskScheduler
    {
    public:
        // Hardware_concurrency gets an estimate of the number of threads available on the system
        ENGINE_API TaskScheduler(size_t thread_count = getThreadCountEstimate());

        ENGINE_API ~TaskScheduler();

        ENGINE_API void submit(const Task& task);

        ENGINE_API void submitBatched(size_t total_items, std::function<void(size_t index)> function_per_item,
                                      size_t desired_tasks = 0);

        ENGINE_API void wait();

        ENGINE_API void setSchedulerIdleThreadSleepTime(long long nanoseconds = 2000000LL);
        ENGINE_API long long getSchedulerIdleThreadSleepTime();

    private:
        void workerLoop(size_t thread_index);
        bool tryStealTask(size_t thief_index, Task& out_task);

        size_t randomIndex() const;

        // Default is 2ms, which feels like a good sweetspot for now
        constexpr static long long WORKER_LOOP_WAIT_NANOSECONDS_DEFAULT = 2000000LL;

        // If desiredTasks is zero in SubmitBatched(), this is the default multiplier for the number of tasks per..
        // worker thread. This allows for better load balancing and more work-stealing. 
        constexpr static unsigned int BATCH_SUBMIT_HEURISTIC_MULTIPLIER = 4;

        long long m_idleSleepTime;
        std::vector<std::unique_ptr<TaskQueue>> m_queues;
        std::vector<std::thread> m_threads;
        std::atomic<bool> m_stopFlag;
        std::condition_variable m_condition;
        std::mutex m_sleepMutex;
        std::mutex m_idleSleepTimeMutex;

        TaskCounter m_taskCounter;
    };
}
