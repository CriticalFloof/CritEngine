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
        ENGINE_API TaskScheduler(size_t threadCount = static_cast<size_t>(GetThreadCountEstimate()));

        ENGINE_API ~TaskScheduler();

        ENGINE_API void Submit(const Task& task);

        ENGINE_API void SubmitBatched(size_t totalItems, std::function<void(size_t index)> functionPerItem, size_t desiredTasks = 0);

        ENGINE_API void Wait();

        ENGINE_API void SetSchedulerIdleThreadSleepTime(const long long nanoseconds = 2000000LL);
        ENGINE_API long long GetSchedulerIdleThreadSleepTime();

    private:

        void WorkerLoop(size_t threadIndex);
        bool TryStealTask(size_t thiefIndex, Task& outTask);

        size_t RandomIndex() const;

        // Default is 2ms, which feels like a good sweetspot for now
        const long long WORKER_LOOP_WAIT_NANOSECONDS_DEFAULT = 2000000LL;

        // If desiredTasks is zero in SubmitBatched(), this is the default multiplier for the number of tasks per..
        // worker thread. This allows for better load balancing and more work-stealing. 
        const unsigned int BATCH_SUBMIT_HUERISTIC_MULTIPLIER = 4;

        long long idleSleepTime;
        std::vector<std::unique_ptr<TaskQueue>> queues;
        std::vector<std::thread> threads;
        std::atomic<bool> stopFlag;
        std::condition_variable condition;
        std::mutex sleepMutex;
        std::mutex idleSleepTimeMutex;

        TaskCounter taskCounter;
    };
}