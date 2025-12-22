#include "TaskScheduler.h"

namespace Engine {

    TaskScheduler::TaskScheduler(size_t threadCount)
        : idleSleepTime(WORKER_LOOP_WAIT_NANOSECONDS_DEFAULT), stopFlag(false)
    {
        ASSERT(threadCount > 0, "Thread count must be greater than zero");

        for (size_t i = 0; i < threadCount; ++i) {
            this->queues.push_back(std::make_unique<TaskQueue>());
        }

        this->threads.reserve(threadCount);

        for (size_t i = 0; i < threadCount; ++i) {
            this->threads.emplace_back([this, i]() { WorkerLoop(i); });
        }
    }

    TaskScheduler::~TaskScheduler()
    {
        this->stopFlag.store(true);
        this->condition.notify_all();

        for (std::thread& thread : this->threads) {
            if (thread.joinable()) thread.join();
        }
    }

    // Submits a new task to the scheduler and updates the internal task counter 
    void TaskScheduler::Submit(const Task& task)
    {
        this->taskCounter.Add(1);

        size_t index = RandomIndex();
        this->queues[index]->PushBack(Task([task, this]() {
            task.Execute();
            this->taskCounter.Signal();
           }));

        this->condition.notify_one();
    }

    // This is a batch helper for when we wanna run any parallel for-loops
    // For very large loops, we want to create a reasonable amount of tasks that process x-x range.
    // So this creates a series of sub-tasks that process chunks of the loop. This pattern helps us take advantage..
    // of our workers.
    void TaskScheduler::SubmitBatched(size_t totalItems, std::function<void(size_t index)> functionPerItem, size_t desiredTasks)
    {
        if (totalItems == 0) return;

        size_t numWorkers = this->threads.size();
        size_t numTasksToCreate = desiredTasks;

        if (numTasksToCreate == 0) {
            // Ensuring there's always *at least* one task but never exceed the number of items.
            // We default to this behavior if desiredTasks isn't set and create a certain amount of tasks per worker thread..
            // to help keep workers busy and productive.
            numTasksToCreate = std::min(totalItems, std::max(static_cast<size_t>(1), numWorkers * BATCH_SUBMIT_HUERISTIC_MULTIPLIER));
        }
        else {
            numTasksToCreate = std::min(totalItems, std::max(static_cast<size_t>(1), numTasksToCreate));
        }

        size_t batchSize = std::max(static_cast<size_t>(1), totalItems / numTasksToCreate);

        for (size_t taskIdx = 0; taskIdx < numTasksToCreate; ++taskIdx) {
            size_t startItemIndex = taskIdx * batchSize;
            size_t endItemIndex = (taskIdx == numTasksToCreate - 1) ? totalItems : std::min(startItemIndex + batchSize, totalItems);

            if (startItemIndex >= endItemIndex) continue;

            Task batchTask([startItemIndex, endItemIndex, functionPerItem]
            {
                for (size_t i = startItemIndex; i < endItemIndex; ++i)
                functionPerItem(i);
            });

            this->Submit(batchTask);
        }
    }

    void TaskScheduler::Wait()
    {
        this->taskCounter.Wait();
    }

    // The main loop executed by every worker thread
    void TaskScheduler::WorkerLoop(size_t threadIndex)
    {
        Task localTask;

        while (!this->stopFlag.load()) {
            // Try to pop a task from this thread's own queue LIFO
            if (auto taskOpt = this->queues[threadIndex]->PopBack()) {
                localTask = *taskOpt;
                localTask.Execute();
            }
            // If our queue is empty, try to steal a task from another thread's queue, which is done FIFO..
            // so the oldest task is always done first
            else if (TryStealTask(threadIndex, localTask)) {
                localTask.Execute();
            }
            // Lastly, if no task is found in the queue, we lock the sleep mutex
            // wait_for() is used instead of wait() to prevent deadlocks if a notification is missed..
            // and to allow periodic rechecking of the stop flag
            else {
                std::unique_lock<std::mutex> lock(this->sleepMutex);
                this->condition.wait_for(lock, std::chrono::nanoseconds(this->idleSleepTime));
                // Calling std::this_thread::yield() is simpler but incurs busy-waiting.. the conditional variable approach..
                // is faster and wastes less resources when idle
            }
        }
    }

    bool TaskScheduler::TryStealTask(size_t thiefIndex, Task& outTask)
    {
        const size_t queueCount = this->queues.size();
        size_t startIndex = RandomIndex();

        for (size_t i = 0; i < queueCount; ++i) {
            size_t victimIndex = (startIndex + i) % queueCount;
            // Don't steal from yourself!
            if (victimIndex == thiefIndex) continue;

            if (auto taskOpt = this->queues[victimIndex]->StealFront()) {
                outTask = *taskOpt;
                return true;
            }
        }
        return false;
    }

    size_t TaskScheduler::RandomIndex() const
    {
        // Each thread gets its own instance of a RNG. This avoids a lot of unnecessary thread contention.
        thread_local static std::mt19937 rng { std::random_device{}() };
        std::uniform_int_distribution<size_t> dist(0, this->queues.size() - 1);
        return dist(rng);
    }

    // Sets the frequency of every wake cycle of an idle thread. Lower numbers mean more busy-waiting and CPU cycles being used when idle.. 
    // but potentially will reduce latency
    void TaskScheduler::SetSchedulerIdleThreadSleepTime(const long long nanoseconds)
    {
        std::lock_guard<std::mutex> lock(this->idleSleepTimeMutex);
        this->idleSleepTime = nanoseconds;
    }

    long long TaskScheduler::GetSchedulerIdleThreadSleepTime()
    {
        std::lock_guard<std::mutex> lock(this->idleSleepTimeMutex);
        return this->idleSleepTime;
    }

}